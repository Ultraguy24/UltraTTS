#include "tts_app.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProcess>
#include <QMessageBox>
#include <QFont>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <QMenuBar>
#include <QAction>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

TTSApp::TTSApp(QWidget* parent) : QWidget(parent) {
    // Set up the window
    setWindowTitle("UltraTTS");
    resize(400, 150);

    // Set a modern font for the entire application
    QFont font("Fira Sans", 10);
    setFont(font);

    // Create widgets
    QLabel* titleLabel = new QLabel("Enter text to speak:", this);
    inputBox = new QLineEdit(this);
    voiceSelector = new QComboBox(this);
    sendButton = new QPushButton("Speak", this);

    // Populate the voice selector from the Piper voices directory
    populateVoiceSelector();

    // Set up the layout
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(inputBox);
    mainLayout->addWidget(voiceSelector);

    // Horizontal layout for the button
    auto buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(sendButton);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    // Create a menubar for the About and Open Folder actions
    auto menuBar = new QMenuBar(this);
    auto helpMenu = menuBar->addMenu("Help");
    auto openFolderAction = new QAction("Open Voices Folder", this);
    auto aboutAction = new QAction("About", this);
    helpMenu->addAction(openFolderAction);
    helpMenu->addAction(aboutAction);

    mainLayout->setMenuBar(menuBar);

    // Connect signals and slots
    connect(sendButton, &QPushButton::clicked, this, &TTSApp::runCommand);
    connect(inputBox, &QLineEdit::returnPressed, this, &TTSApp::runCommand);
    connect(openFolderAction, &QAction::triggered, this, &TTSApp::openVoicesFolder);
    connect(aboutAction, &QAction::triggered, this, &TTSApp::showAboutDialog);

    // Cleanup existing modules from a previous run
    cleanupExistingModules();

    // Setup the virtual microphone on launch
    setupVirtualMic();

    // Connect cleanup to the application's quit signal
    connect(QApplication::instance(), &QApplication::aboutToQuit, this, &TTSApp::cleanupVirtualMic);
}

TTSApp::~TTSApp() {
    // The cleanup will be handled by the aboutToQuit signal.
}

void TTSApp::populateVoiceSelector() {
    QString voicesPath = QDir::homePath() + "/.local/share/ultratts/voices";
    QDir voicesDir(voicesPath);

    if (!voicesDir.exists()) {
        if (voicesDir.mkpath(voicesPath)) {
            QMessageBox::information(this, "Directory Created", "The UltraTTS voices directory has been created. Please add your voice models to it.");
        } else {
            QMessageBox::warning(this, "Directory Error", "The UltraTTS voices directory does not exist and could not be created. Please check your permissions.");
            return;
        }
    }

    QStringList voiceFiles = voicesDir.entryList(QStringList() << "*.onnx", QDir::Files);

    for (const QString& file : voiceFiles) {
        QString voiceName = file;
        voiceName.remove(".onnx");
        voiceSelector->addItem(voiceName, QVariant(voicesPath + "/" + file));
    }

    if (voiceFiles.isEmpty()) {
        QMessageBox::warning(this, "No Voices Found", "No Piper voice files (.onnx) were found in the directory. Please add at least one model to the created directory.");
    }
}

int TTSApp::getSampleRate() {
    QString voicePath = voiceSelector->currentData().toString();
    QString jsonPath = voicePath;
    jsonPath.append(".json");

    QFile file(jsonPath);
    if (!file.open(QIODevice::ReadOnly)) {
        return 22050; // Fallback to a common sample rate
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (doc.isObject()) {
        QJsonObject root = doc.object();
        if (root.contains("audio")) {
            QJsonObject audio = root["audio"].toObject();
            return audio["sample_rate"].toInt();
        }
    }

    return 22050; // Fallback
}

void TTSApp::openVoicesFolder() {
    QString path = QDir::homePath() + "/.local/share/ultratts/voices";
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void TTSApp::showAboutDialog() {
    QMessageBox::about(this, "About UltraTTS",
                       "<h2>UltraTTS v1.0.0</h2>"
                       "<p>A simple graphical front-end for the Piper Text-to-Speech engine.</p>"
                       "<p>Built by ultraguy.</p>"
                       "<p>Uses the Piper and PulseAudio command-line tools.</p>"
                       "<p>GitHub: <a href=\"https://github.com/Ultraguy24/UltraTTS\">https://github.com/Ultraguy24/UltraTTS</a></p>");
}

void TTSApp::runCommand() {
    QString text = inputBox->text();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter some text.");
        return;
    }

    // Get the selected voice file path from the combo box
    QString selectedVoicePath = voiceSelector->currentData().toString();
    if (selectedVoicePath.isEmpty()) {
        QMessageBox::warning(this, "Voice Selection Error", "No voice is selected.");
        return;
    }

    int sampleRate = getSampleRate();

    // The command to run with piper-tts and the correct output flag
    QString program = "sh";
    QStringList arguments;

    // Play to the ultratts_sink
    QString command = QString("echo \"%1\" | piper-tts --model \"%2\" --output-file - | paplay --device=ultratts_sink --format=s16le --rate=%3 --channels=1")
                             .arg(text)
                             .arg(selectedVoicePath)
                             .arg(sampleRate);
    arguments << "-c" << command;

    // Create and start the process
    QProcess* process = new QProcess(this);
    process->start(program, arguments);

    // Clear the input box after the command is issued
    inputBox->clear();
}

void TTSApp::cleanupExistingModules() {
    QProcess process;
    process.start("pactl", QStringList() << "list" << "short" << "modules");
    process.waitForFinished();

    QString output = process.readAllStandardOutput();
    QStringList lines = output.split('\n');

    for (const QString& line : lines) {
        if (line.contains("ultratts_sink") || line.contains("tts_sink") ||
            line.contains("ultratts_source") || line.contains("tts_source")) {

            QStringList parts = line.simplified().split(' ');
            if (parts.count() > 0) {
                QString moduleID = parts.at(0);
                QProcess unloadProcess;
                unloadProcess.start("pactl", QStringList() << "unload-module" << moduleID);
                unloadProcess.waitForFinished();
                qDebug() << "Unloaded old module:" << moduleID;
            }
        }
    }
}

void TTSApp::setupVirtualMic() {
    QProcess process;

    // Create the "black hole" sink and store its module ID
    process.start("pactl", QStringList() << "load-module" << "module-null-sink" << "sink_name=ultratts_sink" << "sink_properties=device.description=\"UltraTTS\"");
    if (process.waitForFinished()) {
        QString ttsSinkModuleId = process.readAllStandardOutput().trimmed();
        m_ttsSinkModuleId = ttsSinkModuleId.toInt();
    }

    // Remap that sink’s monitor as a source (microphone) and store its module ID
    process.start("pactl", QStringList() << "load-module" << "module-remap-source" << "master=ultratts_sink.monitor" << "source_name=ultratts_source" << "source_properties=device.description=\"UltraTTS\"");
    if (process.waitForFinished()) {
        QString ttsSourceModuleId = process.readAllStandardOutput().trimmed();
        m_ttsSourceModuleId = ttsSourceModuleId.toInt();
    }
}

void TTSApp::cleanupVirtualMic() {
    // Unload the remapped source first
    if (m_ttsSourceModuleId != -1) {
        QProcess process;
        process.start("pactl", QStringList() << "unload-module" << QString::number(m_ttsSourceModuleId));
        process.waitForFinished();
    }

    // Unload the sink
    if (m_ttsSinkModuleId != -1) {
        QProcess process;
        process.start("pactl", QStringList() << "unload-module" << QString::number(m_ttsSinkModuleId));
        process.waitForFinished();
    }
}