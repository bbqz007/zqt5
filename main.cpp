#include <QtWidgets>
#include <QtCore>

#include "zqt_helper.h"

using namespace zhelper::qt5Widgets;
#define TRIGGERED (const std::function<void()>&)
#define ONSIGNAL std::make_pair
#define SETTING(T) (const std::function<void(T*)>&) [=](T* target)
#define ONLOAD layout::onload ,

class MyMainWindow : public QMainWindow
{
public:
    MyMainWindow(QWidget* parent = NULL);
private:
    QSharedPointer<QSettings> ini_;
    QHash<qint64, QSharedPointer<QProcess> > bgprocs_;
    QHash<qint64, QString> bgcmds_;
};


MyMainWindow::MyMainWindow(QWidget* parent)
 :QMainWindow(parent)
{
    QMainWindow* win = this;
    ini_.reset(new QSettings(
                    QApplication::applicationDirPath() + "/zqt_smailtoolkit.ini",
                    QSettings::IniFormat));

    auto hisdialog = new QDialog(win);
    auto p = new QFrame(win);
    auto mainlayout = new QVBoxLayout(p);
    setMenuBar(
        menu::begin(new QMenuBar)
            (menu::begin("&File")
                ("E&xit")
                    [TRIGGERED [=](){
                        ini_->sync();
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
                    [TRIGGERED [=]() {
                            hisdialog->show();
                        }]
                (menu::end))
            (menu::end)
    );

    auto acceptDragFile =
        [](QDragEnterEvent* event){
            if (event->mimeData()->hasFormat("text/uri-list"))
            event->acceptProposedAction();
        };

    auto acceptDropFile =
        [=](const QString& group, QLineEdit* edit, QDropEvent* event){
            if(!edit)
                return;
        };
    auto loadIni =
        [=](const QString& group, QLineEdit* edit) {
            if(!edit)
                return;
        };
    auto saveIni =
        [=](const QString& group, QLineEdit* edit, const QString& value) {
            if(!edit)
                return;
        };

    auto runBackground =
        [=](QSharedPointer<QProcess> bg, const QString& cmd, QTextEdit* log)
        {
            QObject::connect(bg.get(),
                &QProcess::started,
                [=]() {
                    qint64 pid = bg->processId();
#if 0
                    QObject::connect(bg.get(),
                        ZQ_SIGNAL<QProcess, int>(&QProcess::finished),
                        [=](int exitCode) {
                            log->append(QString("%1: ExitCode(%2).\n").arg(pid).arg(exitCode));
                            bgprocs_[pid].reset();
                         });
#else
                    ZQ_CONNECT(bg.get(), &QProcess::finished,
                        (lambda_transfer(),
                        [=](int exitCode) {
                            log->append(QString("%1: ExitCode(%2).\n").arg(pid).arg(exitCode));
                            bgprocs_[pid].reset();
                         }));
#endif
                });
            bg->start("cmd /c " + cmd);
            QTableWidget* table = hisdialog->findChild<QTableWidget*>("history");
            using namespace tblwdgthlp;
            row::begin(table)
                (QString("%1").arg(bg->processId()))
                    [bit_and, (Qt::ItemFlags)~Qt::ItemIsEnabled]
                    [QColor("darkBlue")]
                (cmd)
                    [bit_and, (Qt::ItemFlags)~Qt::ItemIsEnabled]
                ("Unknown")
                ("Unknown")
                (row::end);
        };

    layout::begin(mainlayout)
        ("Environment:",
         layout::begin(new QVBoxLayout)
            [QMargins(0, 0, 0, 0)]
            (layout::begin(new QHBoxLayout)
                (new ZQ<QLineEdit>)
                    ["path_to_java"]
                    [ONLOAD [=](QLineEdit* edit){
                            edit->setReadOnly(true);
                            loadIni("Environment", edit);
                        }]
                    [ONSIGNAL (&ZQEmitter::dragEnterEvent,
                               acceptDragFile
                        ) ]
                    [ONSIGNAL (&ZQEmitter::dropEvent,
                        [=](QDropEvent* event){
                            acceptDropFile("Environment",
                                           win->findChild<QLineEdit*>("path_to_java"),
                                           event);
                        }) ]
                (new QPushButton)
                    ["java..."]
                    [ONSIGNAL (&QPushButton::clicked,
                        [=](bool){
                            QUrl url = QFileDialog::getOpenFileUrl(win,
                                                                   "path_to_java", QUrl(),
                                                                   "java.exe (*.exe)");
                            saveIni("Environment",
                                    win->findChild<QLineEdit*>("path_to_java"),
                                    url.toLocalFile());
                        }) ]
                (layout::end))
            (layout::begin(new QHBoxLayout)
                (new ZQ<QLineEdit>)
                    ["path_to_boot_oat"]
                    [ONLOAD [=](QLineEdit* edit){
                            edit->setReadOnly(true);
                            loadIni("Environment", edit);
                        }]
                    [ONSIGNAL (&ZQEmitter::dragEnterEvent,
                               acceptDragFile
                        ) ]
                    [ONSIGNAL (&ZQEmitter::dropEvent,
                        [=](QDropEvent* event){
                            acceptDropFile("Environment",
                                       win->findChild<QLineEdit*>("path_to_boot_oat"),
                                       event);
                        }) ]
                (new QPushButton)
                    ["boot..."]
                    [ONSIGNAL (&QPushButton::clicked,
                        [=](bool){
                            QUrl url = QFileDialog::getOpenFileUrl(win,
                                                                   "path_to_boot_oat", QUrl(),
                                                                   "boot.oat (*.oat)");
                            saveIni("Environment",
                                    win->findChild<QLineEdit*>("path_to_boot_oat"),
                                    url.toLocalFile());
                        }) ]
                (layout::end))
            (layout::begin(new QHBoxLayout)
                (new ZQ<QLineEdit>)
                    ["path_to_smali_jar"]
                    [ONLOAD [=](QLineEdit* edit){
                            edit->setReadOnly(true);
                            loadIni("Environment", edit);
                        }]
                    [ONSIGNAL (&ZQEmitter::dragEnterEvent,
                               acceptDragFile
                        ) ]
                    [ONSIGNAL (&ZQEmitter::dropEvent,
                        [=](QDropEvent* event){
                            acceptDropFile("Environment",
                                   win->findChild<QLineEdit*>("path_to_smali_jar"),
                                   event);
                        }) ]
                (new QPushButton)
                    ["smali..."]
                    [ONSIGNAL (&QPushButton::clicked,
                        [=](bool){
                            QUrl url = QFileDialog::getOpenFileUrl(win,
                                                                   "path_to_smali_jar", QUrl(),
                                                                   "smali.jar (*.jar)");
                            saveIni("Environment",
                                    win->findChild<QLineEdit*>("path_to_smali_jar"),
                                    url.toLocalFile());
                        }) ]
                (layout::end))
            (layout::begin(new QHBoxLayout)
                (new ZQ<QLineEdit>)
                    ["path_to_baksmali_jar"]
                    [ONLOAD [=](QLineEdit* edit){
                            edit->setReadOnly(true);
                            loadIni("Environment", edit);
                        }]
                    [ONSIGNAL (&ZQEmitter::dragEnterEvent,
                               acceptDragFile
                        ) ]
                    [ONSIGNAL (&ZQEmitter::dropEvent,
                        [=](QDropEvent* event){
                            acceptDropFile("Environment",
                               win->findChild<QLineEdit*>("path_to_baksmali_jar"),
                               event);
                        }) ]
                (new QPushButton)
                    ["baksmali..."]
                    [ONSIGNAL (&QPushButton::clicked,
                        [=](bool){
                            QUrl url = QFileDialog::getOpenFileUrl(win,
                                                                   "path_to_baksmali_jar", QUrl(),
                                                                   "baksmali.jar (*.jar)");
                            saveIni("Environment",
                                    win->findChild<QLineEdit*>("path_to_baksmali_jar"),
                                    url.toLocalFile());
                        }) ]
                (layout::end,
                 layout::oncomplete =
                    [](QLayout*) {

                    }))
            (layout::end))
        ("Execution:",
         layout::begin(new QVBoxLayout)
            [QMargins(0, 0, 0, 0)]
            (layout::begin(new QHBoxLayout)
                (new ZQ<QLineEdit>)
                    ["path_to_odex"]
                    [ONSIGNAL (&QLineEdit::textChanged,
                        [=]() {

                        })]
                    [ONSIGNAL (&ZQEmitter::dragEnterEvent,
                               acceptDragFile
                        ) ]
                    [ONSIGNAL (&ZQEmitter::dropEvent,
                        [=](QDropEvent* event){
                            acceptDropFile("Execution",
                               win->findChild<QLineEdit*>("path_to_odex"),
                               event);
                        }) ]
                (new QPushButton)
                    ["odex2smali"]
                    [ONSIGNAL (&QPushButton::clicked,
                        [=](bool){
                            QSharedPointer<QProcess> proc(new QProcess);
                            QTextEdit* log = win->findChild<QTextEdit*>("output_log");
                            QString cmd("notepad");
                            runBackground(proc, cmd, log);
                            bgcmds_[proc->processId()] = cmd;
                            bgprocs_[proc->processId()] = proc;
                            log->append(QString("%1: %2\n").arg(QString::number(proc->processId()), cmd));
                        }) ]
                (layout::end))
            (layout::begin(new QHBoxLayout)
                (new ZQ<QLineEdit>)
                    ["path_to_smali_dir"]
                    [ONSIGNAL (&QLineEdit::textChanged,
                        [=]() {

                        })]
                    [ONSIGNAL (&ZQEmitter::dragEnterEvent,
                               acceptDragFile
                        ) ]
                    [ONSIGNAL (&ZQEmitter::dropEvent,
                        [=](QDropEvent* event){
                            acceptDropFile("Execution",
                               win->findChild<QLineEdit*>("path_to_smali_dir"),
                               event);
                        }) ]
                (new QPushButton)
                    ["smali2dex"]
                    [ONSIGNAL (&QPushButton::clicked,
                        [=](bool){
                            QSharedPointer<QProcess> proc(new QProcess);
                            QTextEdit* log = win->findChild<QTextEdit*>("output_log");
                            QString cmd("notepad");
                            runBackground(proc, cmd, log);
                            bgcmds_[proc->processId()] = cmd;
                            bgprocs_[proc->processId()] = proc;
                            log->append(QString("%1: %2\n").arg(proc->processId()).arg(cmd));
                        }) ]
                (layout::end))
            (layout::end))
        ("Output Log:",
         layout::begin(new QVBoxLayout)
            [QMargins(0, 0, 0, 0)]
            (layout::begin(new QHBoxLayout)
                (new QTextEdit)
                    ["output_log"]
                    [ONLOAD [=](QTextEdit* log) {
                        log->setReadOnly(true);
                        log->resize(log->width(), 300);
                        log->append("welcome to zhelper.(a gui4smali demo written with zqt_helper)\n");
                    } ]
                (layout::end))

            (layout::end))

        (new QWidget,
         layout::begin(new QHBoxLayout)
            [QMargins(0, 0, 0, 0)]
            (new QPushButton(tr("1")))
                [ONLOAD [=](QPushButton* btn) {
                        btn->setText(tr("1st"));
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
            mainlayout->setStretch(2, 1);
            p->setLayout(mainlayout);
            win->setCentralWidget(p);
            win->setWindowTitle("zhelper gui4smali (zqt version)");
        });

    /* -----===== layout dialog Frame =====-------- */
    layout::begin(new QVBoxLayout)
        [QMargins(0, 0, 0, 0)]
        (new QTableWidget)
            ["history"]
            [ONLOAD [=](QTableWidget* table) {
                using namespace tblwdgthlp;
                column::begin(table)
                    ("pid")
                    ("cmd")
                    ("col 3") [QColor("red")]
                    ("col 4") [QColor("red")]
                    (column::end);
                table->setColumnWidth(1, 400);

            }]
        (layout::end,
         layout::oncomplete =
            [=](QLayout* s) {
                hisdialog->setLayout(s);
                hisdialog->setWindowTitle("history");
                hisdialog->setMinimumWidth(700);
         });
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QSharedPointer<MyMainWindow> sptr_win(new MyMainWindow);

    sptr_win->show();

    return app.exec();
}
