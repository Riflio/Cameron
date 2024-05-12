#ifndef DEFINES_H
#define DEFINES_H


//-- узнаём количество параметров
#define _W_NARGS_OVERRIDE(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, N, ...) N
#define _W_NARGS(...) _W_NARGS_OVERRIDE(__VA_ARGS__, 10, 9, 8, 7, 6 ,5, 4, 3, 2, 1, 0)


//-- получаем конкретный параметр из списка
#define _W_GETPARAM1(_1,...) _1
#define _W_GETPARAM2(_1,_2,...) _2
#define _W_GETPARAM3(_1,_2,_3,...) _3


//-- подготавливаем параметр для дефайнов
#define _W_ARG_OVERRIDE(_1, _2, _3, NAME, ...) NAME
#define _W_ARG2(a,b) AA(a,b)
#define _W_ARG3(a,b,c) AA(a,b,c)
#define W_ARG(...) _W_ARG_OVERRIDE(__VA_ARGS__, _W_ARG3, _W_ARG2)(__VA_ARGS__)

//-- от переданного типа и переменной нам нужо только название переменной
#define _W_ARGNAME_AA(a,b, ...) b
#define _W_ARGNAME_ //-- нам не передали переменную
#define _W_ARGNAME(a) _W_ARGNAME_##a

//-- т.к. к нам приходит значение уже в скобках, нужно их раскрыть
#define __W_ARGPREP2(a,b,...) a b
#define __W_ARGPREP3(a,b,c,...) a b c

//-- нам нужны только тип и сама переменная
#define _W_ARGPREP2_AA(...) __W_ARGPREP2(__VA_ARGS__)
#define _W_ARGPREP2_ //-- нам не передали переменную
#define _W_ARGPREP2(a) _W_ARGPREP2_##a

//-- нам нужны тип, сама переменная, и всё, что после (если есть)
#define _W_ARGPREP3_AA(...) _W_ARG_OVERRIDE(__VA_ARGS__, __W_ARGPREP3, __W_ARGPREP2)(__VA_ARGS__)
#define _W_ARGPREP3_ //-- нам не передали переменную
#define _W_ARGPREP3(a) _W_ARGPREP3_##a

//-- вызываем дефайн с каждым параметром
#define _W_CALL1(DEF,a) DEF(a)
#define _W_CALL2(DEF,a,b) DEF(a), DEF(b)
#define _W_CALL3(DEF,a,b,c) DEF(a) , DEF(b) , DEF(c)
#define _W_CALL4(DEF,a,b,c,d) DEF(a) , DEF(b) , DEF(c) , DEF(d)
#define _W_CALL5(DEF,a,b,c,d,e) DEF(a) , DEF(b) , DEF(c) , DEF(d) , DEF(e)
#define _W_CALL6(DEF,a,b,c,d,e,f) DEF(a) , DEF(b) , DEF(c) , DEF(d) , DEF(e) , DEF(f)
#define _W_CALL7(DEF,a,b,c,d,e,f,g) DEF(a) , DEF(b) , DEF(c) , DEF(d) , DEF(e) , DEF(f), DEF(g)
#define _W_CALL8(DEF,a,b,c,d,e,f,g,h) DEF(a) , DEF(b) , DEF(c) , DEF(d) , DEF(e) , DEF(f), DEF(g), DEF(h)
#define _W_CALL9(DEF,a,b,c,d,e,f,g,h,i) DEF(a) , DEF(b) , DEF(c) , DEF(d) , DEF(e) , DEF(f), DEF(g), DEF(h), DEF(i)
#define _W_CALL10(DEF,a,b,c,d,e,f,g,h,i,j) DEF(a) , DEF(b) , DEF(c) , DEF(d) , DEF(e) , DEF(f), DEF(g), DEF(h), DEF(i), DEF(j)

//-- вызываем дефайн в зависимости от количества параметров
#define _W_CALL_OVERRIDE(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME, ...) NAME
#define _W_CALL(DEF,...) _W_CALL_OVERRIDE(__VA_ARGS__, _W_CALL10,_W_CALL9,_W_CALL8,_W_CALL7,_W_CALL6,_W_CALL5,_W_CALL4,_W_CALL3,_W_CALL2,_W_CALL1)(DEF,__VA_ARGS__)

//-- определяем функцию, виртуальный указатель на неё и функцию для вызова его
#define W_FUNCT(className, qa, retType, functName, cw, ...) \
        qa retType functName( _W_CALL(_W_ARGPREP3, __VA_ARGS__) ) cw { return (this->*__##functName)( _W_CALL(_W_ARGNAME, __VA_ARGS__) );  } \
        retType _##functName(  _W_CALL(_W_ARGPREP3, __VA_ARGS__) ) cw; \
        retType (className::*__##functName)( _W_CALL(_W_ARGPREP2, __VA_ARGS__)  ) cw;

//-- задаём значение указателю функции на его функцию
#define W_FUNCT_REL(className, functName, classPointer) classPointer->__##functName = & className::_##functName;


//-- просто представляем переданное строкой
#define _W_STRING(X) #X

//-- что бы могли использовать запятушку в параметрах дефайнов
#define COMM ,

//-- Что бы автоматически отключать слот от сигнала после уничтожения указателя на соединение (полезно вместе с QEventLoop, connect и лямбда-функцией)
#define AutoDisconnect(l) \
    QSharedPointer<QMetaObject::Connection> l = QSharedPointer<QMetaObject::Connection>(\
        new QMetaObject::Connection(), \
        [](QMetaObject::Connection * conn) { /*QSharedPointer сам по себе не производит отключения при удалении*/ \
            QObject::disconnect(*conn);\
        }\
    ); *l /* Use AutoDisconnect(conn1) = connect(....); */

#endif // DEFINES_H
