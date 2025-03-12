#ifndef LOCALSTART_H
#define LOCALSTART_H

#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QRandomGenerator>
#include <QVBoxLayout>
#include <QWidget>

// Third screen class
class LocalStart : public QWidget {
  Q_OBJECT

 public:
  explicit LocalStart(QWidget* parent = nullptr);

  // Setter functions to accept the data
  void setRedTeamSpyMaster(const QString& name);
  void setRedTeamOperative(const QString& name);
  void setBlueTeamSpyMaster(const QString& name);
  void setBlueTeamOperative(const QString& name);

 private:
  QLabel* redSpyMasterLabel;
  QLabel* redOperativeLabel;
  QLabel* blueSpyMasterLabel;
  QLabel* blueOperativeLabel;
};

#endif  // LOCALSTART_H