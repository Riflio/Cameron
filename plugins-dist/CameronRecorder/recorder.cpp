#include "recorder.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>

Recorder::Recorder(QObject *parent, ICamera *camera, const TRecorderSettings &settings): QObject(parent), _camera(camera),_settings(settings)
{
  //-- Создаём сам поток записи
  _thread =new QThread(this);
  _recorderThread =new RecorderThread();
  connect(_thread, &QThread::started, _recorderThread, &RecorderThread::onStarted);

  //-- Будем проверять по таймеру пора ли новый файл записи (по ограничениям из настроек). Здесь, что бы не нагружать проверками сам поток записи.
  _makeNewRecFileIfNeedTimer =new QTimer(this);
  _makeNewRecFileIfNeedTimer->setInterval(1000); //TODO: В настройки время проверки
  connect(_makeNewRecFileIfNeedTimer, &QTimer::timeout, this, &Recorder::makeNewRecFileIfNeed);
}

Recorder::~Recorder()
{    
  qDebug()<<"";
  Recorder::stopRec();
  if ( _recorderThread!=nullptr ) { delete _recorderThread; _recorderThread =nullptr; }
}

/**
* @brief Запускаем запись
* @return
*/
bool Recorder::startRec()
{
  qInfo()<<"";
  //-- Запустим камеру
  if ( !_camera->go() ) { return false; }
  //-- Установим
  if ( !_recorderThread->setCamera(_camera) ) { return false; }
  //-- Сразу задаём информацию о файле записи
  if ( checkNeedNewFile() ) { if ( !makeNewRecFile() ) { return false; } }
  else { setRecorderThreadNewRecFileInfo(_curFileInfo); }

  //-- Запускаем автопроверку и создание нового файла записи
  _makeNewRecFileIfNeedTimer->start();

  //-- Запустим наш поток записи
  if ( _thread->isRunning() ) { return true; }
  _thread->start();

  //-- Всё ок
  return true;
}

/**
* @brief Останавливаем запись
*/
bool Recorder::stopRec()
{
  qInfo()<<"";
  _makeNewRecFileIfNeedTimer->stop();
  //-- Отключаем камеру
  _camera->stop();
  //-- Останавливаем наш поток записи
  if ( !_thread->isRunning() ) { return true; }
  _thread->quit();
  _thread->wait();
  //-- Всё ок
  return true;
}

/**
* @brief Инициализация всего необходимого
* @return
*/
bool Recorder::init()
{
  if ( !loadFilesList() ) { return false; }
  return true;
}

/**
* @brief Отдаём наш идентификатор
* @return
*/
quint32 Recorder::id() const
{
  return _settings.id;
}

/**
* @brief Отадём инфу о всех наших файлах
* @return
*/
QMap<quint64, TRecFileInfo> Recorder::filesInfo() const
{
  return _filesList;
}

/**
* @brief Отдаём инфу о текущем файле
* @return
*/
TRecFileInfo Recorder::fileInfo() const
{
  if ( _curFileInfo==nullptr ) { return TRecFileInfo(); }
  return *_curFileInfo;
}

/**
* @brief При ошибке камеры
*/
void Recorder::onCameraError()
{
  //-- Останавливаем запись
  stopRec();
  //FIXME: Перезапуск камеры
}

/**
* @brief При ошибке потока самописца
*/
void Recorder::onRecorderThreadError()
{
  //FIXME: Перезапускаем поток?
}

/**
 * @brief Задаём самописцу новую информацию о файле, в который писать
 * @param recFileInfo
 */
void Recorder::setRecorderThreadNewRecFileInfo(TRecFileInfo *recFileInfo)
{
  QMetaObject::invokeMethod(_recorderThread, "setRecFileInfo",Qt::QueuedConnection, Q_ARG(TRecFileInfo *, recFileInfo));
}

/**
* @brief Проверяем, нужен ли новый файл для записи
* @return
*/
bool Recorder::checkNeedNewFile() const
{
  //-- Если вообще нет активного файла записи
  if ( _curFileInfo==nullptr ) { return true; }
  //-- Если размер уже превышает максимальный
  if ( _settings.maxFileSize>0 && _curFileInfo->size>=_settings.maxFileSize ) { return true; }
  //-- Если файл создан до времени после которого нужно создать новый файл
  QDateTime newFileAt =QDateTime(QDate::currentDate(), _settings.newFileAtTime.addSecs(-1)); //-- -1 что бы при времени 0:0:0 был ещё пошлый день
  if ( _settings.newFileAtTime.isValid() && _curFileInfo->dt<newFileAt && newFileAt<QDateTime::currentDateTime() ) { return true; }
  return false;
}

/**
* @brief Устанавливаем новый файл для записи самописцу
*/
bool Recorder::makeNewRecFile()
{
  qDebug()<<"";

  //-- Если есть ограничение на максимальное количество файлов - перезапись по кругу
  if ( _settings.maxFilesCount>0 && _filesList.count()>=_settings.maxFilesCount ) {
    TRecFilesList::Iterator fileIt =std::find(_filesList.begin(), _filesList.end(), *_curFileInfo);
    //-- Попробуем найти следующий за активным, если при этом не кончились ещё по списку
    if ( fileIt!=_filesList.end() && (fileIt =std::next(fileIt, 1))!=_filesList.end() ) {
      _curFileInfo =&fileIt.value();
    } else {
      _curFileInfo =&_filesList.begin().value();
    }
    _curFileInfo->size =0; //-- Что бы начал заново несмотря на текущий размер файла в системе
    _curFileInfo->dt =QDateTime::currentDateTime();
  } else { //-- Новый
    uint64_t newFileId =_filesList.count();
    QString filePath =QString("%1/Recorder%2_rec-%3.h264").arg(_settings.savePath).arg(_settings.id).arg(newFileId); //TODO: В настройки формат пути
    _filesList[newFileId] =TRecFileInfo{.id =newFileId, .path =filePath, .size =0, .dt =QDateTime::currentDateTime()};
    _curFileInfo =&_filesList[newFileId];
  }
  if ( !saveFilesList() ) { return false; }

  //-- Сразу задаём его потоку самописца. Если с этим возникнут проблемы - он сгенерирует ошибку
  setRecorderThreadNewRecFileInfo(_curFileInfo);

  //-- Всё ок
  return true;
}

/**
* @brief Устанавливаем новый файл для записи самописцу, если он нужен
* @return true если не нужен или нужен и задали успешно. При ошибке будет сигнал.
*/
bool Recorder::makeNewRecFileIfNeed()
{
  //-- Не нужен новый
  if ( !checkNeedNewFile() ) {
    //-- Если не нужен новый, то хотя бы периодически обновим информацию о текущем
    saveFilesList();
    return true;
  }
  //-- Нужен новый
  saveFilesList(); //-- Сохраним финальную инфу о текущем файле
  return makeNewRecFile();
}

/**
* @brief Прогрузим список записанных нами файлов
* @return
*/
bool Recorder::loadFilesList()
{
  _filesListFile.setFileName(QString("%1/Recorder%2_%3.xml").arg(_settings.savePath).arg(_settings.id).arg("rec")); //TODO: В настройки формат пути

  if ( !_filesListFile.open(QIODevice::ReadWrite | QIODevice::Text) ) {
    qWarning()<< "Unable open records info file"<<_filesListFile.fileName()<<"!";
    return false;
  }

  _filesListFile.reset();
  _filesList.clear();

  int curID =-1;

  QXmlStreamReader xml(&_filesListFile);
  while ( !xml.atEnd()  ) {
    if ( xml.hasError() ) { qWarning()<<"Records info file has error syntax"<<xml.name(); return false; }
    QXmlStreamReader::TokenType token =xml.readNext();
    if ( (token==QXmlStreamReader::StartDocument) || (token==QXmlStreamReader::EndDocument) ) { continue; }
    if ( token==QXmlStreamReader::StartElement ) {
      if ( xml.name()==QStringLiteral("file") )  {
        TRecFileInfo rf;
        rf.path =xml.attributes().value("path").toString();
        rf.size =xml.attributes().value("size").toUInt();
        rf.id =xml.attributes().value("fileID").toUInt();
        rf.dt =QDateTime::fromString(xml.attributes().value("created").toString(), "dd.MM.yyyy hh:mm:ss");
        //-- Если файла фактически не существует уже, то пропускаем
        if ( !QFile::exists(rf.path) ) { qWarning()<<"Record file with patn"<<rf.path<<"not exists - skipped"; continue; }
        _filesList[rf.id] =rf;
      } else
      if ( xml.name()==QStringLiteral("cur") ) {
        curID =xml.attributes().value("fileID").toUInt();
      }
    }
  }

  if ( curID>=0 && _filesList.contains(curID) ) {
    _curFileInfo =&_filesList[curID];
  }

  return true;
}

/**
* @brief Сохраним список записанных нами файлов
* @return
*/
bool Recorder::saveFilesList()
{
  if ( !_filesListFile.isOpen() ) { return false; }
  _filesListFile.resize(0);

  //-- Записываем всё из _files
  QXmlStreamWriter xml(&_filesListFile);
  xml.writeStartElement("files");
  foreach (TRecFileInfo rf, _filesList) {
    xml.writeStartElement("file");
      xml.writeAttribute("ripperID", QString::number(_settings.id));
      xml.writeAttribute("path", rf.path);
      xml.writeAttribute("size", QString::number(rf.size));
      xml.writeAttribute("created", rf.dt.toString("dd.MM.yyyy hh:mm:ss"));
      xml.writeAttribute("fileID", QString::number(rf.id));
    xml.writeEndElement();
  }

  if ( _curFileInfo!=nullptr ) {
    xml.writeStartElement("cur");
      xml.writeAttribute("fileID", QString::number(_curFileInfo->id));
    xml.writeEndElement();
  }
  xml.writeEndElement();

  _filesListFile.flush();

  return true;
}
