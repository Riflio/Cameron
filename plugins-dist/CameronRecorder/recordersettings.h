#ifndef RECORDERSETTINGS_H
#define RECORDERSETTINGS_H

#include <QString>
#include <QDateTime>

/// @brief Настройки конкретного самописца
struct TRecorderSettings
{
  int id =0; ///< Уникальный идентификатор
  int camID =0; ///< Идентификатор камеры, от которой брать поток для записи
  QString savePath =""; ///< Пусть куда сохранять
  bool autoStart =true; ///< Начинать записть автоматически при старте (или управляется извне другими плагинами)
  quint64 maxFileSize =0; ///< Максимальный размер одного файла (в Мб)
  int maxFilesCount =0; ///< Максимальное количество файлов, после которого пойдёт перезапись по кругу существующих
  QTime newFileAtTime =QTime(); ///< Время при котором будет создан новый или перезаписан файл (в зависимости от maxFilesCount)
};

#endif // RECORDERSETTINGS_H
