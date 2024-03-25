#ifndef LAZYSHARK_HEXDUMPDIALOG_H
#define LAZYSHARK_HEXDUMPDIALOG_H


#include <QDialog>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>

class HexDumpDialog : public QDialog {
public:
    explicit HexDumpDialog(const QString &data, QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("Packet Data");

        auto *textEdit = new QTextEdit(this);
        textEdit->setFontFamily("Courier"); // Set to a monospaced font
        textEdit->setFontPointSize(12);     // Set a comfortable font size
        textEdit->setReadOnly(true);        // Make it read-only
        textEdit->setText(data);            // Set the text to your hex dump data
        textEdit->setFixedSize(800, 600);
        textEdit->setWordWrapMode(QTextOption::NoWrap);

        auto *closeButton = new QPushButton("Close", this);
        connect(closeButton, &QPushButton::clicked, this, &HexDumpDialog::reject);

        auto *layout = new QVBoxLayout(this);
        layout->addWidget(textEdit);
        layout->addWidget(closeButton);
    }
};

#endif //LAZYSHARK_HEXDUMPDIALOG_H
