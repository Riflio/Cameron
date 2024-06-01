#ifndef TRECFILE_H
#define TRECFILE_H

#include <QString>
#include <QDateTime>

/// @brief Информация о записанном файле
struct TRecFileInfo {
  uint64_t id =0; ///< Уникальный идентификатор
  QString path; ///< Путь
  size_t size =0; ///< Размер в байтах
  QDateTime dt; ///< Дата и время создания  

  bool operator==(const TRecFileInfo& other) const {
    return id==other.id;
  }
};

#endif // TRECFILE_H
