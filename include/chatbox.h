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
    enum Team {
        RED_TEAM,
        BLUE_TEAM
    };
    explicit ChatBox(const QString& playerName, Team team, QWidget* parent = nullptr);
    ~ChatBox();
    void addSystemMessage(const QString& message, Team team);
    void addPlayerMessage(const QString& playerName, const QString& message);

public slots:
    void sendMessage();

private:
    Team team;
    QTextEdit* chatDisplay;
    QLineEdit* chatInput;
    QPushButton* sendButton;
    QString playerName;
};

#endif // CHATBOX_H