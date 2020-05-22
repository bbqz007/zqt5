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
#include <functional>

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

template<class T>
class layout_builder
{
public:
    T* t_;
    QWidget* w_;
    layout_builder(T* t) : t_(t), w_(NULL)
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
    layout_builder& operator [] (const std::function<void(Y*)>& config)
    {
        Y* y = dynamic_cast<Y*>(w_);
        if (y)
            config(y);
        else if(y = dynamic_cast<Y*>(t_))
            config(y);
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
            QObject::connect((typename QtPrivate::FunctionPointer<Signal>::Object*) w_, ss.first, ss.second);
        return *this;
    }
    layout_builder& operator [] (const QString& s)
    {
        if (w_)
            setText(s, w_);
        else
            setText(s, t_);
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
        //t_->addWidget(y);
        return *this;
    }
    template<typename Y>
    layout_builder& operator () (QLayout*, Y* y)
    {
        finish_prev_widget();
        //QWidget* gb = new QWidget;
        //gb->setLayout(y);
        t_->addItem(y);
        //t_->addWidget(gb);
        //t_->setContentsMargins(0, 0, 0, 0);
        //gb->setFlat(true);
        return *this;
    }
    void setText(const QString& s, QObject* o)
    {
        o->setObjectName(s);
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
        }
    }

};

class layout
{
public:
    static layout_end end;
    template<class T>
    static layout_builder<T>& begin(T* t) { return * new layout_builder<T>(t); }
};
layout_end layout::end;

};
};

#endif // ZQT_HELPER__H_
