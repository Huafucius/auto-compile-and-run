#include "runbutton.h"

RunButtonWidget::RunButtonWidget(QWidget *parent)
    : QWidget(parent), executablePath("") {
    button = new QPushButton("Run Executable", this);
    outputArea = new QTextEdit(this);
    outputArea->setReadOnly(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(button);
    layout->addWidget(outputArea);

    connect(button, &QPushButton::clicked, this, &RunButtonWidget::runExecutable);
}

void RunButtonWidget::setExecutablePath(const QString &path) {
    executablePath = path;
}

void RunButtonWidget::runExecutable() {
    if (executablePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "Executable path is not set.");
        return;
    }

    QProcess process;
    process.start(executablePath);

    if (!process.waitForStarted()) {
        QMessageBox::critical(this, "Error", "Failed to start executable.");
        return;
    }

    if (!process.waitForFinished()) {
        QMessageBox::critical(this, "Error", "Failed to execute executable.");
        return;
    }

    QString output = process.readAllStandardOutput();
    QString errorOutput = process.readAllStandardError();

    if (!errorOutput.isEmpty()) {
        outputArea->append("Error:\n" + errorOutput);
    }

    outputArea->append("Output:\n" + output);
}
