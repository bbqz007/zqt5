#include <QtWidgets>

#include "zqt_helper.h"

using namespace zhelper::qt5Widgets;
#define TRIGGERED (const std::function<void()>&)
#define ONSIGNAL std::make_pair
#define SETTING(T) (const std::function<void(T*)>&) [=](T* target)

class MyMainWindow : public QMainWindow
{
public:
    MyMainWindow(QWidget* parent = NULL);
};
MyMainWindow::MyMainWindow(QWidget* parent)
 :QMainWindow(parent)
{
    QMainWindow* win = this;

    auto p = new QFrame(win);
    auto mainlayout = new QVBoxLayout(p);
    setMenuBar(
        menu::begin(new QMenuBar)
            (menu::begin("&File")
                ("E&xit")
                    [TRIGGERED [win](){
                        win->close();
                    }]
                ("&About")
                        [TRIGGERED []() {
                            QMessageBox::about(NULL,
                                                "about",
                                                "https://github.com/bbqz007\nhttp://www.cnblogs.com/bbqzsl");
                        }]
                (menu::end))
            (menu::begin("&Advance")
                ("&History")
                    [TRIGGERED []() {
                            QMessageBox::about(NULL,
                                                "about",
                                                "https://github.com/bbqz007\nhttp://www.cnblogs.com/bbqzsl");
                        }]
                (menu::end))
            (menu::end)
    );
    layout::begin(mainlayout)
        ("Environment:",
         layout::begin(new QVBoxLayout)
            [QMargins(0, 0, 0, 0)]
            (layout::begin(new QHBoxLayout)
                (new QLineEdit)
                (new QPushButton)
                (layout::end))
            (layout::begin(new QHBoxLayout)
                (new QLineEdit)
                (new QPushButton)
                (layout::end))
            (layout::begin(new QHBoxLayout)
                (new QLineEdit)
                (new QPushButton)
                (layout::end))
            (layout::begin(new QHBoxLayout)
                (new QLineEdit)
                (new QPushButton)
                (layout::end))
            (layout::end))
        ("Execution:",
         layout::begin(new QVBoxLayout)
            [QMargins(0, 0, 0, 0)]
            (layout::begin(new QHBoxLayout)
                (new QLineEdit)
                (new QPushButton)
                (layout::end))
            (layout::begin(new QHBoxLayout)
                (new QLineEdit)
                (new QPushButton)
                (layout::end))
            (layout::end))
        ("Output Log:",
         layout::begin(new QVBoxLayout)
            [QMargins(0, 0, 0, 0)]
            (layout::begin(new QHBoxLayout)
                (new QTextEdit)
                (layout::end))

            (layout::end))
        (new QWidget,
         layout::begin(new QHBoxLayout)
            [QMargins(0, 0, 0, 0)]
            (new QPushButton(tr("1")))
                [SETTING (QPushButton) {
                        target->setText(tr("1st"));
                    } ]
                [ONSIGNAL (&QPushButton::clicked,
                    [](bool){
                        QMessageBox::information(NULL, "", "clicked 1");
                    }) ]
                [ONSIGNAL (&QPushButton::toggled,
                    [](bool){
                        QMessageBox::information(NULL, "", "toggled 1");
                    }) ]
            (new QPushButton(tr("2")))
                [ONSIGNAL (&QPushButton::clicked,
                    [](bool){
                        QMessageBox::information(NULL, "", "clicked 2");
                    }) ]
            (new QPushButton(tr("3")))
                [ONSIGNAL (&QPushButton::clicked,
                    [](bool){
                        QMessageBox::information(NULL, "", "clicked 3");
                    }) ]
            (layout::end))
        (new QPushButton)
        ("",
         layout::begin(new QVBoxLayout)
            [QMargins(0, 0, 0, 0)]
            (layout::begin(new QHBoxLayout)
            (new QLineEdit(tr("4")))
            (new QPushButton(tr("5")))
              (layout::end))
            (layout::begin(new QHBoxLayout)
            (new QLineEdit(tr("4")))
            (new QPushButton(tr("5")))
              (layout::end))
            (layout::end))
        (layout::begin(new QHBoxLayout)

            (layout::end))
        (layout::end, [=](QLayout*) {
            mainlayout->setStretch(3, 1);
            p->setLayout(mainlayout);
            win->setCentralWidget(p);
        });
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QSharedPointer<MyMainWindow> sptr_win(new MyMainWindow);

    sptr_win->show();

    return app.exec();
}
