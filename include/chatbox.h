#ifndef CHATBOX_H
#define CHATBOX_H

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>

class ChatBox : public QWidget {
    Q_OBJECT

public:
    explicit ChatBox(const QString& playerName, QWidget* parent = nullptr);
    ~ChatBox();
    void addSystemMessage(const QString& message);
    void addPlayerMessage(const QString& playerName, const QString& message);

public slots:
    void sendMessage();

private:
    QTextEdit* chatDisplay;
    QLineEdit* chatInput;
    QPushButton* sendButton;
    QString playerName;
};

#endif // CHATBOX_H