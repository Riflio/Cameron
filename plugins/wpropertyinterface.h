#ifndef IWPROPERTYINTERFACE_H
#define IWPROPERTYINTERFACE_H

#include <QObject>
#include <QVariant>
#include <QMap>
#include <QList>

/**
* Даём возможность в Q_PROPERTY исользовать указатели на абстрактные классы (интерфейсы)
* а так же почти любой QMap<type, type>.
* ключ должен иметь возможность конвертиться в строку, а значение в QVariant
* а так же почти любой QList<type>,
* тип должен либо быть указателем на QObject либо простым типом
*
* USE:
* class MyInterface
* {
* Q_GADGET
* public:
* ...
* };
* Q_DECLARE_METATYPE(MyInterface*)
* Q_DECLARE_METATYPE(WPropertyInterface<MyInterface*>)
*
* class MyClass: public QObject  {
*   Q_OBJECT
*   Q_PROPERTY(WPropertyInterface<MyInterface*> myProperty READ myProperty)
* public:
*   MyInterface* myProperty() {
*       ....
*   }
* };
*
*/

template<class T>
class WPropertyInterface: public QVariant
{
public:
    WPropertyInterface():QVariant() { }

    WPropertyInterface(T p):QVariant() {
        initByQObjectPointer(p);
    }

    /**
    * @brief Конвертируем из QMap
    */
    template <class QMK, class QMV>
    WPropertyInterface & operator =(const QMap<QMK,QMV> & map) {
        initByQMap(map);
        return *this;
    }

    /**
    * @brief Конвертируем из указателя в QObject
    */
    template <class OP>
    WPropertyInterface  & operator =(OP * p)
    {
        initByQObjectPointer(p);
        return *this;
    }

    /**
    * @brief Конвертируем из QList
    */
    template<class QMV>
    WPropertyInterface &operator =(const QList<QMV> & list)
    {
        initByQList(list);
        return * this;
    }

    /**
    * @brief Обратно в QMap
    */
    template <class QMK, class QMV>
    operator QMap<QMK, QMV>() {
        QVariantMap vm = toMap();
        QMap<QMK, QMV> map;

        QMapIterator<QString, QVariant> i(vm);
        while (i.hasNext()) {
            i.next();
            map.insert( QVariant(i.key()).value<QMK>(), i.value().value<QMV>() );
        }
        return map;
    }

    /**
    * @brief Обратно в QMap, если есть указатели - кастуем из QObject
    */
    template <class QMK, class QMV>
    operator QMap<QMK, QMV*>() {
        QVariantMap vm = toMap();
        QMap<QMK, QMV*> map;

        QMapIterator<QString, QVariant> i(vm);
        while (i.hasNext()) {
            i.next();
            map.insert( QVariant(i.key()).value<QMK>(), dynamic_cast<QMV*>(i.value().value<QObject*>()));
        }
        return map;
    }

    /**
    * @brief Обратно в QList<QMV>
    */
    template<class QMV>
    operator QList<QMV>() {
        QVariantList vl = toList();
        QList<QMV> list;

        QListIterator<QVariant> i(vl);
        while (i.hasNext()) {
            list.append(i.next().value<QMV>());
        }
        return list;
    }

    /**
    * @brief Обратно в QList, если есть указатели - кастуем из QObject
    */
    template<class QMV>
    operator QList<QMV*>() {
        QVariantList vl = toList();
        QList<QMV*> list;

        QListIterator<QVariant> i(vl);
        while (i.hasNext()) {
            list.append( dynamic_cast<QMV*>(i.next().value<QObject*>()));
        }
        return list;
    }

    /**
    * @brief Обратно к исходному из QObject*
    */
    template<class OP>
    operator OP*(){
        return dynamic_cast<OP*>(value<QObject*>());
    }

    /**
    * @brief Каст к какому-то без указателя, юзаем копирование, а там пусть сам решает
    */
    template<class OP>
    operator OP() {
        T t = (*this);
        return OP(t);
    }


private:

    /**
    * @brief
    */
    template <class QMK, class QMV>
    void initByQMap(const QMap<QMK, QMV> & map)
    {
        QVariantMap vm;
        QMapIterator<QMK, QMV> i(map);
        while (i.hasNext()) {
            i.next();
            vm.insert(QVariant(i.key()).toString(), qVariantFromValue(i.value()));
        }

        create(qMetaTypeId<QVariantMap>(), &vm);
    }

    /**
    * @brief Если значение - указатель на Q_OBJECT или Q_GADGET, то кастуем сразу к QObject*
    */
    template <class QMK, class QMV>
    void initByQMap(const QMap<QMK, QMV*> & map)
    {
        QVariantMap vm;
        QMapIterator<QMK, QMV*> i(map);

        int typeFlags = QMetaType::typeFlags(qMetaTypeId<QMV*>());
        bool isPointerTOQObject = ( (typeFlags&QMetaType::PointerToGadget) || (typeFlags&QMetaType::PointerToQObject) );

        while (i.hasNext()) {
            i.next();
            QVariant v = (isPointerTOQObject)?  qVariantFromValue(dynamic_cast<QObject*>(i.value())) : qVariantFromValue(i.value());
            vm.insert(QVariant(i.key()).toString(), v);
        }

        create(qMetaTypeId<QVariantMap>(), &vm);
    }


    /**
    * @brief
    */
    template <class QMV>
    void initByQList(const QList<QMV> & list)
    {
        QVariantList vl;
        QListIterator<QMV> i(list);

        while (i.hasNext()) {
            vl.append(qVariantFromValue(i.next()));
        }

        create(qMetaTypeId<QVariantList>(), &vl);
    }

    /**
    * @brief
    */
    template <class QMV>
    void initByQList(const QList<QMV*> & list)
    {
        QVariantList vl;
        QListIterator<QMV*> i(list);

        int typeFlags = QMetaType::typeFlags(qMetaTypeId<QMV*>());
        bool isPointerTOQObject = ( (typeFlags&QMetaType::PointerToGadget) || (typeFlags&QMetaType::PointerToQObject) );

        while (i.hasNext()) {
            QVariant v = (isPointerTOQObject)? qVariantFromValue(dynamic_cast<QObject*>(i.next())) : qVariantFromValue(i.next());
            vl.append(v);
        }

        create(qMetaTypeId<QVariantList>(), &vl);
    }


    /**
    * @brief
    */
    template <class OP>
    void initByQObjectPointer(OP * p)
    {
        QObject * obj = dynamic_cast<QObject*>(p);
        create(qMetaTypeId<QObject*>(), &obj);
    }

};

//-- Прикинемся встроенным для Qt типом, почему - смотреть  qv4qobjectwrapper.cpp (loadProperty), qv4engine.cpp (fromVariant), в итоге придём к qqmlpropertycache_p.h (Flags::QVariantType)

template <class QMK, class QMV>
struct QMetaTypeId2<WPropertyInterface<QMap<QMK, QMV> > >
{
    enum {
        Defined = true,
        IsBuiltIn=false
    };
    static inline Q_DECL_CONSTEXPR int qt_metatype_id() {
        return  QMetaType::QVariantMap;
    }
};

template <class QMV>
struct QMetaTypeId2<WPropertyInterface<QList<QMV> > >
{
    enum {
        Defined = true,
        IsBuiltIn=false
    };
    static inline Q_DECL_CONSTEXPR int qt_metatype_id() {
        return  QMetaType::QVariantList;
    }
};

template <class T>
struct QMetaTypeId2<WPropertyInterface<T> >
{
    enum {
        Defined = true,
        IsBuiltIn=false
    };
    static inline Q_DECL_CONSTEXPR int qt_metatype_id() {
        return  QMetaType::QObjectStar;
    }
};

#endif // IWPROPERTYINTERFACE_H
