#ifndef SPYMASTERHINT_H
#define SPYMASTERHINT_H

#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>

class SpymasterHint : public QWidget {
  Q_OBJECT

 public:
  explicit SpymasterHint(QWidget* parent = nullptr);
  ~SpymasterHint();

 signals:
  void hintSubmitted(const QString& hint, const int number);

 private slots:
  void submitHint();
  void updateButtonClickable();

 private:
  QLineEdit* hintLineEdit;
  QSpinBox* numberSpinBox;
  QPushButton* giveClueButton;
};


#endif // SPYMASTERHINT_H
