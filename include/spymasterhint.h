#ifndef SPYMASTERHINT_H
#define SPYMASTERHINT_H

#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QRegularExpressionMatchIterator>

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
  void textToUppercase(const QString& text);

 private:
  QLineEdit* hintLineEdit;
  QSpinBox* numberSpinBox;
  QPushButton* giveClueButton;
  QRegularExpressionMatchIterator* textValidator;
};


#endif // SPYMASTERHINT_H
