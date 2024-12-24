#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QWidget>
#include <QLabel>
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
    void updateSubtitle(const QString &subtitleText);  // Slot to update subtitle text

private:
    Ui::MainWindow *ui;
    void openVideo(const QString &filePath); // Declaration of openVideo function
    QString getSubtitleAtTime(const QString &srtFilePath, int currentTimeInSeconds); // Subtitle parsing function

    // libVLC player-related members
    libvlc_instance_t *vlcInstance = nullptr;
    libvlc_media_player_t *mediaPlayer = nullptr;
    libvlc_media_t *media = nullptr;
    QWidget *videoWidget = nullptr;
    QVBoxLayout *layout = nullptr;

    // Subtitle display widget
    QLabel *subtitleLabel = nullptr;  // QLabel to display subtitles
};

#endif // MAINWINDOW_H
