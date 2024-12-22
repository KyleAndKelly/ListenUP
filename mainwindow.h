#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QWidget>
#include <QVBoxLayout>
#include <vlc/vlc.h>
#include <vlc/libvlc.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openButton_clicked();

private:
    Ui::MainWindow *ui;
    void openVideo(const QString &filePath); // Declaration of openVideo function

    // libVLC player-related members
    libvlc_instance_t *vlcInstance = nullptr;
    libvlc_media_player_t *mediaPlayer = nullptr;
    libvlc_media_t *media = nullptr;
    QWidget *videoWidget = nullptr;
    QVBoxLayout *layout = nullptr;
};

#endif // MAINWINDOW_H
