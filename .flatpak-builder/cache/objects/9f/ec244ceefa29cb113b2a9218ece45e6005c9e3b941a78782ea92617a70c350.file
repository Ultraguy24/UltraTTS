#ifndef TTS_APP_H
#define TTS_APP_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QProcess>

class TTSApp : public QWidget {
    Q_OBJECT

public:
    explicit TTSApp(QWidget* parent = nullptr);
    ~TTSApp() override;

private slots:
    void populateVoiceSelector();
    int getSampleRate();
    void openVoicesFolder();
    void showAboutDialog();
    void runCommand();

private:
    void cleanupExistingModules(); // <-- Add this line
    void setupVirtualMic();
    void cleanupVirtualMic();

    QLineEdit* inputBox;
    QPushButton* sendButton;
    QComboBox* voiceSelector;
    int m_ttsSinkModuleId = -1;
    int m_ttsSourceModuleId = -1;
};

#endif // TTS_APP_H