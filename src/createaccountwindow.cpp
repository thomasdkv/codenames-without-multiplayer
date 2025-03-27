#include "createaccountwindow.h"

CreateAccountWindow* CreateAccountWindow::instance = nullptr;

CreateAccountWindow::CreateAccountWindow(QWidget* parent) : QWidget(parent) {
  // Set up the window
  setWindowTitle("Create Account");
  this->setFixedSize(1000, 600);

  // Create the layout
  QVBoxLayout* layout = new QVBoxLayout(this);

  // Create a horizontal layout for the top area
  QHBoxLayout* topLayout = new QHBoxLayout();

  // Create the "Back to Main Menu" button
  QPushButton* backButton = new QPushButton("Back", this);
  topLayout->addWidget(backButton);

  // Add the "Back" button layout to the main layout
  layout->addLayout(topLayout);

  // Create title label
  QLabel* titleLabel = new QLabel("Create Account", this);
  titleLabel->setAlignment(Qt::AlignCenter);
  layout->addWidget(titleLabel);

  // Create username input field
  usernameEdit = new QLineEdit(this);
  usernameEdit->setPlaceholderText("Enter username");
  layout->addWidget(usernameEdit);

  // Create the "Create Account" button
  createAccountButton = new QPushButton("Create Account", this);
  layout->addWidget(createAccountButton);

  // Create label to show account status
  statusLabel = new QLabel(this);
  statusLabel->setAlignment(Qt::AlignCenter);
  layout->addWidget(statusLabel);

  // Connect the button click signal to a slot
  connect(createAccountButton, &QPushButton::clicked, this,
          &CreateAccountWindow::onCreateAccountClicked);

  // // Connect the back button to navigate to the main menu
  connect(backButton, &QPushButton::clicked, this,
          &CreateAccountWindow::goBack);

  // Set the layout for the window
  setLayout(layout);
}

CreateAccountWindow* CreateAccountWindow::getInstance(QWidget* parent) {
  if (instance == nullptr) {
    instance = new CreateAccountWindow(parent);
  }
  return instance;
}

void CreateAccountWindow::goBack() {
  this->hide();
  emit back();
}

void CreateAccountWindow::onCreateAccountClicked() {
  // Placeholder logic for creating account, update the status label
  QString username = usernameEdit->text().trimmed();
  if (username.isEmpty()) {
    statusLabel->setText("Please enter a username.");
  } else {
    // For now, just show that the account is created
    statusLabel->setText("Account Created.");
    saveJsonFile(username);
  }
}

// Save JSON data
void CreateAccountWindow::saveJsonFile(const QString& username) {
  QFile file(jsonFilePath);
  QDir dir = QFileInfo(file).absoluteDir();
  QString absolutePath = dir.filePath(file.fileName());

  QJsonObject jsonObject;

  // Read existing JSON data (if any)
  if (file.exists()) {
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      qDebug() << "Failed to open" << absolutePath << " for reading.";
      statusLabel->setText("Error: Could not read profile.json");
      return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isObject()) {
      jsonObject = doc.object();  // Load existing data
    }
  }

  // Check if user exists and preserve statistics
  QJsonObject userObject;
  QJsonObject statistics;

  if (jsonObject.contains(username)) {
    statusLabel->setText("Account " + username + " already exists");
    userObject = jsonObject[username].toObject();
    if (userObject.contains("statistics")) {
      statistics =
          userObject["statistics"].toObject();  // Preserve existing statistics
    }
  }

  // Initialize statistics if they don't exist
  if (statistics.isEmpty()) {
    statistics["games_played"] = 0;
    statistics["games_win"] = 0;
    statistics["guess_total"] = 0;
    statistics["guess_hit"] = 0;
  }

  // Update user object
  userObject["user_name"] = username;
  userObject["statistics"] = statistics;

  // Store the updated user object in the main JSON
  jsonObject[username] = userObject;

  // Write updated JSON data
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qDebug() << "Failed to write to" << absolutePath;
    statusLabel->setText("Error: Could not write to profile.json");
    return;
  }

  QJsonDocument newDoc(jsonObject);
  file.write(newDoc.toJson());
  file.close();
}
