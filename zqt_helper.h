/**
MIT License

Copyright (c) 2020 bbqz007 <https://github.com/bbqz007, http://www.cnblogs.com/bbqzsl>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef ZQT_HELPER__H_
#define ZQT_HELPER__H_

#include <qsharedpointer.h>
#include <qaction.h>
#include <QtWidgets>
#include <functional>

/**
* !!!!!
* functions and classes in the file
* they do not take ownership of any your widget, object,...
*/

namespace zhelper
{
namespace qt5Widgets
{

class menu_end {};
class layout_end {};

template<class T>
class menu_builder
{
public:
    T* t_;
    QAction* action_;
    std::function<void(QAction*)> callback_;
    menu_builder(T* t) : t_(t), action_(NULL), callback_(NULL) {}
    menu_builder& operator () (const QString& s)
    {
        finish_prev_action();
        action_ = new QAction(s);
        return *this;
    }
    menu_builder& operator () (QMenu* sub)
    {
        finish_prev_action();
        action_ = NULL;
        t_->addMenu(sub);
        return *this;
    }
    menu_builder& operator [] (const std::function<void(QAction*)>& f)
    {
        callback_ = f;
        return *this;
    }
    menu_builder& operator [] (const std::function<void()>& trigger)
    {
        if (action_)
            QObject::connect(action_, &QAction::triggered, trigger);
        return *this;
    }
    T* operator () (menu_end&)
    {
        finish_prev_action();
        return t_;
    }
    T* operator () (menu_end&, const std::function<void(T*)>& complete)
    {
        finish_prev_action();
        complete(t_);
        T* ret = t_;
        delete this;
        return ret;
    }
private:
    void finish_prev_action()
    {
        if (action_)
        {
            if (callback_)
                callback_(action_);
            t_->addAction(action_);
        }
        action_ = NULL;
        callback_ = NULL;
    }
};

class menu
{
public:
    static menu_end end;
    static menu_builder<QMenu>& begin(QMenu* t)
    {
        return * new menu_builder<QMenu>(t);
    }
    static menu_builder<QMenuBar>& begin(QMenuBar* t)
    {
        return * new menu_builder<QMenuBar>(t);
    }
    static menu_builder<QMenu>& begin(const QString& s)
    {
        return * new menu_builder<QMenu>(new QMenu(s));
    }
    static menu_builder<QMenu>& begin(QWidget* w)
    {
        return * new menu_builder<QMenu>(new QMenu(w));
    }
};

menu_end menu::end;

class layout_context
{
public:
    QWidget*& operator [](const int& s)
    {
        return wc_[s];
    }
private:
    QMap<int, QWidget*> wc_;
};

template<class T, char a=' ', char b=' ', char c=' ', char d=' '>
class prop_assign
{
public:
    prop_assign() {}
    prop_assign(T& t) : t_(t) {}
    T t_;
    prop_assign operator =(T& t)
    {
        return prop_assign(t);
    }
};

typedef prop_assign<int, 'i', 'd'> T_Prop_id; T_Prop_id Prop_id;

class ZQEmitter;
template<class T> class layout_builder;
// route the widget event handles to signal-slot

class ZQEmitter : public QObject
{
    Q_OBJECT
public:
    Q_SIGNALS:
    void leaveEvent(QEvent* event);  // do not switch window here.
    void dropEvent(QDropEvent* event);
    void dragEnterEvent(QDragEnterEvent *event);
};
#include "zqt_helper_moc.h"

template<class QW>
class ZQ : public QW
{
public:
    template<typename ... Types>
    ZQ(Types... args) : QW(args...) {}

protected:
    virtual void leaveEvent(QEvent* event)
    {
        QW::leaveEvent(event);
        if (event->isAccepted())
            Q_EMIT emit_.leaveEvent(event);

    }
    virtual void dropEvent(QDropEvent* event)
    {
        Q_EMIT emit_.dropEvent(event);
    }
    virtual void dragEnterEvent(QDragEnterEvent *event)
    {
        Q_EMIT emit_.dragEnterEvent(event);
    }
private:
    template<class T> friend class layout_builder;
    ZQEmitter emit_;
};

class lambda_transfer
{
public:
    // thanks to a solution of deduce types of arguments and result of lambda to make std::function
    //   by https://stackoverflow.com/questions/21245891/deduce-template-argument-when-lambda-passed-in-as-a-parameter
    template<typename F>
    auto operator , (const F& f)
    {
        return operator ()(f, &F::operator());
    }
private:
    template<typename F, typename T>
    std::function<void(T*)> operator () (const F& f, void(F::*)(T*) const)
    {
        return std::function<void(T*)>(f);
    }
    template<typename F, typename ... Args>
    std::function<void(Args...)> operator () (const F& f, void(F::*)(Args...) const)
    {
        return std::function<void(Args...)>(f);
    }
};
// use ZQ_SIGNAL<T, Args...> to solve a overloaded function
template<typename T, typename ... Args>
auto ZQ_SIGNAL(void(T::*f)(Args... ) )
{
    return f;
}
template<typename T, typename F, typename ... Args, typename Functor>
void ZQ_CONNECT_IMPL(T* t, void(T::*f)(Args...),
                     const Functor& functor, void(Functor::*)(Args...))
{
    QObject::connect(t, f, functor);
}
template<typename T, typename F, typename Functor>
void ZQ_CONNECT(T* t, F* f, const Functor& functor)
{
    // it does not work, because of 'f' of a unsolved overloaded function
    ZQ_CONNECT_IMPL(t, f, functor, &Functor::operator());
}
template<typename T, typename ... Args>
void ZQ_CONNECT(T* t, void(T::*f)(Args...), const std::function<void(Args...)>& functor)
{
    QObject::connect(t, f, functor);
}

///////////////////////////

class QIdString : public QString
{
public:
    QIdString& operator= (const QString& s)
    {
        QString::operator=(s);
        /// clang bug https://stackoverflow.com/questions/25147667/clang-produces-illegal-instruction-where-gcc-doesnt
        return *this;
    }
    QIdString& operator= (QString&& s)
    {
        QString::operator=(std::move(s));
        return *this;
    }
};

__thread
struct
{
    QIdString operator= (const QString& s)
    {
        QIdString idstr;
        idstr = s;
        return idstr;
    }
    QIdString operator= (QString&& s)
    {
        QIdString idstr;
        idstr = std::move(s);
        return idstr;
    }
} id;

struct onclick_bridge;
template<typename Widget>
struct onclick_prop
{
    std::function<void(Widget*)> f_;
private:
    onclick_prop() {}
    friend struct onclick_bridge;
};

__thread
struct onclick_bridge
{
    template<typename Widget>
    auto operator= (const std::function<void(Widget*)>& f)
    {
        onclick_prop<Widget> prop;
        prop.f_ = f;
        return prop;
    }
    template<typename Functor>
    auto operator= (const Functor& functor)
    {
        return operator= ((lambda_transfer(), functor));
    }
    auto& operator= (const std::function<void()>& f)
    {
        return f;
    }
} onclick;

__thread
struct onload_bridge
{
    template<typename Functor>
    auto operator= (const Functor& functor)
    {
        return (lambda_transfer(), functor);
    }
} onload;

struct onsignal_bridge;

template<typename Signal>
struct onsignal_prop
{
    Signal signal_;

    template<typename Functor>
    auto operator= (const Functor& functor)
    {
        return std::make_pair(signal_, functor);
    }
    template<typename Functor>
    auto operator, (const Functor& functor)
    {
        return std::make_pair(signal_, functor);
    }
private:
    onsignal_prop() : signal_(0) {}
    friend struct onsignal_bridge;
};

__thread
struct onsignal_bridge
{
    template<typename Signal>
    auto operator[] (Signal sig)
    {
        onsignal_prop<Signal> prop;
        prop.signal_ = sig;
        return prop;
    }
    template<typename Signal>
    auto operator() (Signal sig)
    {
        onsignal_prop<Signal> prop;
        prop.signal_ = sig;
        return prop;
    }
    template<typename Signal>
    auto operator= (Signal sig)
    {
        onsignal_prop<Signal> prop;
        prop.signal_ = sig;
        return prop;
    }
} onsignal;

struct onchar_bridge;
template<typename Widget>
struct onchar_prop
{
    std::function<void(Widget*, const QString&)> f_;
private:
    onchar_prop() {}
    friend struct onchar_bridge;
};

__thread
struct onchar_bridge
{
    template<typename Widget>
    auto operator= (const std::function<void(Widget*, const QString&)>& f)
    {
        onchar_prop<Widget> prop;
        prop.f_ = f;
        return prop;
    }
    template<typename Functor>
    auto operator= (const Functor& functor)
    {
        return operator= ((lambda_transfer(), functor));
    }
} onchar;

///////////////////////////

template<class T>
class layout_builder
{
public:
    T* t_;
    QWidget* w_;
    ZQEmitter* e_;
    layout_builder(T* t) : t_(t), w_(NULL), e_(NULL)
    {
    }
    template<typename Y>
    layout_builder& operator () (Y* y)
    {
        return (*this)(y, y);
    }
    layout_builder& operator () (const QString& s, QLayout* y)
    {
        finish_prev_widget();
        QGroupBox* gb = new QGroupBox(s);
        gb->setLayout(y);
        t_->addWidget(gb);
        return *this;
    }
    layout_builder& operator () (QWidget* gb, QLayout* y)
    {
        finish_prev_widget();
        gb->setLayout(y);
        t_->addWidget(gb);
        return *this;
    }
    template<typename Y>
    layout_builder& operator [] (const std::function<void(Y*)>& onload)
    {
        Y* y = dynamic_cast<Y*>(w_);
        if (!y)
            y = dynamic_cast<Y*>(t_);
        if (y)
            onload(y);
        return *this;
    }
    layout_builder& operator [] (const QMargins& m)
    {
        t_->setContentsMargins(m);
        return *this;
    }
    template<class Signal, class Slot>
    layout_builder& operator [] (const std::pair<Signal, Slot>& ss)
    {
        if (w_)
        {
            typedef typename QtPrivate::FunctionPointer<Signal>::Object Tsender;
            Tsender* sender = dynamic_cast<Tsender*>(w_);
            if (!sender)
                sender = dynamic_cast<Tsender*>(e_);
            if (sender)
                QObject::connect(sender, ss.first, ss.second);
                //connect(sender, ss);
        }

        return *this;
    }
    template<typename Widget>
    layout_builder& operator [] (const onclick_prop<Widget>& op)
    {
        /// deprecated
        auto f = op.f_;
        typedef typename QtPrivate::FunctionPointer<decltype(&Widget::clicked)>::Object Tsender;
        Tsender* sender = 0;
        if (w_)
        {
            sender = dynamic_cast<Tsender*>(w_);
            if (!sender)
                sender = dynamic_cast<Tsender*>(e_);
        }
        return operator [] (std::make_pair(&Widget::clicked, [=](bool){ f((Widget*)sender); }));
    }
    template<typename Widget>
    layout_builder& operator [] (const onchar_prop<Widget>& op)
    {
        /// deprecated
        auto f = op.f_;
        typedef typename QtPrivate::FunctionPointer<decltype(&Widget::textChanged)>::Object Tsender;
        Tsender* sender = 0;
        if (w_)
        {
            sender = dynamic_cast<Tsender*>(w_);
            if (!sender)
                sender = dynamic_cast<Tsender*>(e_);
        }
        return operator [] (std::make_pair(&Widget::textChanged, [=](const QString& s){ f((Widget*)sender, s); }));
    }
    layout_builder& operator [] (const QString& s)
    {
        if (w_)
            setText(s, w_);
        else
            setText(s, t_);
        return *this;
    }
    layout_builder& operator [] (const QIdString& s)
    {
        if (w_)
            setId((const QString&)s, w_);
        else
            setId((const QString&)s, t_);
        return *this;
    }

    T* operator () (layout_end&)
    {
        finish_prev_widget();
        T* ret = t_;
        delete this;
        return ret;
    }
    T* operator () (layout_end&, const std::function<void(QLayout*)>& complete)
    {
        finish_prev_widget();
        T* ret = t_;
        complete(t_);
        delete this;
        return ret;
    }
private:
    layout_builder& operator () (QWidget*, QWidget* y)
    {
        finish_prev_widget();
        w_ = y;
        // we defer to t_->addWidget(y);
        return *this;
    }
    template<class Y>
    layout_builder& operator () (QWidget*, ZQ<Y>* y)
    {
        finish_prev_widget();
        w_ = y;
        e_ = &y->emit_;
        // we defer to t_->addWidget(y);
        return *this;
    }
    template<typename Y>
    layout_builder& operator () (QLayout*, Y* y)
    {
        finish_prev_widget();
#if 1
        t_->addItem(y);
#else
        //QWidget* gb = new QWidget;
        //gb->setLayout(y);
        //t_->addWidget(gb);
        //t_->setContentsMargins(0, 0, 0, 0);
        //gb->setFlat(true);
#endif
        return *this;
    }
    template<class Sender, class Signal, class Slot>
    void connect(Sender* sender, const std::pair<Signal, Slot>& ss)
    {
        QObject::connect(sender, ss.first, ss.second);
    }
    void setId(const QString& s, QObject* o)
    {
        o->setObjectName(s);
    }
    void setId(const QString& s, QGroupBox* o)
    {
        /// do nothing
    }
    void setText(const QString& s, QObject* o)
    {
        o->setObjectName(s);
        {
            QLineEdit* edit = dynamic_cast<QLineEdit*>(o);
            (edit) ? edit->setText(s) : (void)edit;
        }
        {
            QAbstractButton* button = dynamic_cast<QAbstractButton*>(o);
            (button) ? button->setText(s) : (void)button;
        }
    }
    void setText(const QString& s, QGroupBox* o)
    {
        o->setTitle(s);
    }
    void finish_prev_widget()
    {
        if (w_)
        {
            t_->addWidget(w_);
            w_ = NULL;
            e_ = NULL;
        }
    }

};

class layout
{
public:
    static layout_end end;
    static std::function<void(QLayout*)> oncomplete;
    static lambda_transfer onload;
    template<class T>
    static layout_builder<T>& begin(T* t) { return * new layout_builder<T>(t); }
};
layout_end layout::end;
std::function<void(QLayout*)> layout::oncomplete;
lambda_transfer layout::onload;

#define DECLARE_BIT_OP_TRANSFER(op) \
class op##_transfer  \
{   \
public: \
    template<class T>   \
    auto operator , (const T& t)    \
    {   \
        return std::bind1st(std::op<T>(), t); \
    }   \
};  \
op##_transfer op;

DECLARE_BIT_OP_TRANSFER(bit_and)


#undef DECLARE_BIT_OP_TRANSFER

/* ----------------------------------------------------*/
namespace tblwdgthlp
{

class cell;
class row;
class column;

class cell_builder;
class row_builder;
class column_builder;

class cell_end {};
class cell_another {};
class row_end {};
class row_another {};
class column_end {};

class cell_builder
{
    // warning qtablewidget take ownership of new item of heap.
public:
    explicit cell_builder(QTableWidget* ctrl, bool modify = false)
        : ctrl_(ctrl), col_(0), row_(0), modify_(modify), inuse_(false),
        info_(0)
    {
        if (!modify)
            info_ = new QTableWidgetItem;
    }
    cell_builder& operator ()(int row, int col)
    {
        modify_if_needed();
        inuse_ = true;
        row_ = row;
        col_ = col;
        if (ctrl_)
        {
            info_ = ctrl_->takeItem(row_, col_);
            if (!info_)
            {
                ctrl_->setItem(row_, col_,
                               info_ = new QTableWidgetItem);
            }
        }
        return *this;
    }
    cell_builder& operator [](const QString& txt)
    {
        info_->setText(txt);
        info_->setToolTip(txt);
        return *this;
    }
    cell_builder& operator [](const QColor& color)
    {
        info_->setForeground(QBrush(color));
        return *this;
    }
    cell_builder& operator [](const QFont& font)
    {
        info_->setFont(font);
        return *this;
    }
    template<template<class T> typename Op>
    cell_builder& operator [](const std::binder1st<Op<Qt::ItemFlags> >& flagop)
    {
        info_->setFlags(flagop(info_->flags()));
        return *this;
    }
    QTableWidgetItem* operator ()(cell_end&)
    {
        QTableWidgetItem* ret = info_;
        delete this;
        return ret;
    }
    QTableWidgetItem* operator ()(cell_another&)
    {
        QTableWidgetItem* ret = info_;
        info_ = new QTableWidgetItem;
        return ret;
    }
private:
    void modify_if_needed()
    {
        if (!modify_ || !inuse_ || !ctrl_)
            return;

        ctrl_->setItem(row_, col_, info_);
        info_ = NULL;
    }
    bool inuse_;
    bool modify_;
    int row_;
    int col_;
    QTableWidgetItem* info_;
    QTableWidget* ctrl_;
};

class cell
{
public:
    static cell_end end;
    static cell_another another;
    static cell_builder& begin(QTableWidget* ctrl) { return *new cell_builder(ctrl, true); }
    static cell_builder& begin() { return *new cell_builder(NULL); }
};
cell_end cell::end;
cell_another cell::another;

class column_builder
{
public:
    column_builder(QTableWidget* ctrl) : ctrl_(ctrl)
    {
        col_ = ctrl->columnCount();
        width_ = 0;
    }
    column_builder& operator ()(QTableWidgetItem* info)
    {
        finish_prev_cell();
        cell_.reset();
        ctrl_->insertColumn(col_);
        ctrl_->setHorizontalHeaderItem(col_++, info);
        return *this;
    }
    column_builder& operator ()(const QString& txt)
    {
        finish_prev_cell();
        if (!cell_)
            cell_.reset(&cell::begin());
        (*cell_)[txt];
        //ctrl_->InsertColumn(col_++, txt);
        return *this;
    }
    column_builder& operator [](const QColor& color)
    {
        if (cell_)
            (*cell_)[color];
        return *this;
    }
    column_builder& operator [](const QFont& font)
    {
        if (cell_)
            (*cell_)[font];
        return *this;
    }
    column_builder& operator [](const int width)
    {
        width_ = width;
        return *this;
    }
    QTableWidget* operator ()(column_end&)
    {
        finish_prev_cell();
        QTableWidget* ret = ctrl_;
        delete this;
        return ret;
    }
private:
    void finish_prev_cell()
    {
        if (!cell_)
            return;

        ctrl_->insertColumn(col_);
        ctrl_->setHorizontalHeaderItem(col_++, cell_->operator ()(cell::another));
        if (width_ > 0)
            ctrl_->setColumnWidth(col_ - 1, width_);
        width_ = 0;
    }

    int col_;
    int width_;
    QTableWidget* ctrl_;
    QSharedPointer<cell_builder> cell_;
};

class column
{
public:
    static column_end end;
    static column_builder& begin(QTableWidget* ctrl) { return *new column_builder(ctrl); }
};
column_end column::end;

class row_builder
{
public:
    row_builder(QTableWidget* ctrl) : ctrl_(ctrl), col_(0)
    {
        row_ = ctrl->rowCount();
    }
    row_builder& operator ()(QTableWidgetItem* info)
    {
        finish_prev_cell();
        cell_.reset();
        if (col_ == 0)
            ctrl_->insertRow(row_);
        ctrl_->setItem(row_, col_++, info);
        return *this;
    }
    row_builder& operator ()(const QString& txt)
    {
        finish_prev_cell();

        if (col_ == 0)
            ctrl_->insertRow(row_);

        // defer to setItem
        if (!cell_)
            cell_.reset(&cell::begin());

        // unlike the zwx version, wxlistctrl item takes the col_,
        //                         qtablewidget item does not.
        cell_->operator ()(row_, col_);
        (*cell_)[txt];

        return *this;
    }
    row_builder& operator [](const QColor& color)
    {
        if (cell_)
            (*cell_)[color];
        return *this;
    }
    row_builder& operator [](const QFont& font)
    {
        if (cell_)
            (*cell_)[font];
        return *this;
    }
    template<class T>
    row_builder& operator [](const T& t)
    {
        if (cell_)
            (*cell_)[t];
        return *this;
    }
    QTableWidget* operator ()(row_end&)
    {
        finish_prev_cell();
        QTableWidget* ret = ctrl_;
        delete this;
        return ret;
    }
    row_builder& operator ()(row_another&)
    {
        finish_prev_cell();
        cell_.reset();
        row_++;
        col_ = 0;
        return *this;
    }
private:
    void finish_prev_cell()
    {
        // finish deferred setItem
        if (!cell_)
            return;
        QTableWidgetItem* info = cell_->operator ()(cell::another);
        // unlike the zwx version
        ctrl_->setItem(row_, col_++, info);
    }

    int row_;
    int col_;
    QTableWidget* ctrl_;
    QSharedPointer<cell_builder> cell_;
};

class row
{
public:
    static row_end end;
    static row_another another;
    static row_builder& begin(QTableWidget* ctrl) { return *new row_builder(ctrl); }
};
row_end row::end;
row_another row::another;

}; // tblwdgthlp

}; // qt5Widgets
}; // zhelper

#endif // ZQT_HELPER__H_
