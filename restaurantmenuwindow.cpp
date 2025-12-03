#include "restaurantmenuwindow.h"
#include "ui_restaurantmenuwindow.h"

RestaurantMenuWindow::RestaurantMenuWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RestaurantMenuWindow)
{
    ui->setupUi(this);

    /* Window & Page Setup */
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

    m_dCurrentPrice = prices.Daves1; // Set default to $12.99 for Dave's #1

    QFont font("Digital-7", 11);
    ui->dollarLabel->setFont(font);
    ui->priceCounter->display(m_dCurrentPrice);

    m_iMealIndex = ui->mealsComboBox->currentIndex();   // Set default to Dave's #1
    m_iDrinkIndex = ui->drinksComboBox->currentIndex(); // Set default to None
    m_iMealQuantity = ui->quantitySpinBox->value();     // Set default to 1

    /* Create function connections */
    connect(ui->orderButton, &QPushButton::clicked, this,
            &RestaurantMenuWindow::PlaceOrderBtnClicked
        );

    connect(ui->mealsComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [&] () {
            double price = CalculateCurrentPrice();
            ui->priceCounter->display(QString::number(price, 'f', 2));
        });

    connect(ui->drinksComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [&] () {
            double price = CalculateCurrentPrice();
            ui->priceCounter->display(QString::number(price, 'f', 2));
        });

    connect(ui->quantitySpinBox, &QSpinBox::valueChanged, this,
        [&] () {
            double price = CalculateCurrentPrice();
            ui->priceCounter->display(QString::number(price, 'f', 2));
        });

    connect(ui->heatSlider, &QSlider::valueChanged, this,
        [=] (int newValue) {
            ChangeHeatLevel(newValue);
    });
}

RestaurantMenuWindow::~RestaurantMenuWindow()
{
    delete ui;
}

//======================================================================================

/*
 * Summary   : This function will start cooking process
 *             and let you know when your order is ready.
 * Parameters: void
 * Return    : void
 */
void RestaurantMenuWindow::PlaceOrderBtnClicked()
{
    /* Need to calculate total time needed to process everything */
    int totalTime = DEFAULT_TIME;
    if (m_iMealQuantity > 1) {
        // Add on extra 10s for each extra meal
        totalTime += ( (m_iMealQuantity - 1) * 10);
    }

    ShowTimerProgressDlg(totalTime, this);
}

//======================================================================================

/*
 * Summary   : This function will determine your current balance,
 *             based on your selection(s)
 *             (ex. Dave's #1 + small soft drink = 12.99 + 2.29).
 * Parameters: void
 * Return    : Updated m_dCurrentPrice based on combobox selections.
 */
double RestaurantMenuWindow::CalculateCurrentPrice()
{
    m_iMealIndex = ui->mealsComboBox->currentIndex();
    m_iDrinkIndex = ui->drinksComboBox->currentIndex();
    m_iMealQuantity = ui->quantitySpinBox->value();

    /* Determine meal price first */
    switch (m_iMealIndex)
    {
        case MealNum1: m_dCurrentPrice = prices.Daves1; break;
        case MealNum2: m_dCurrentPrice = prices.Daves2; break;
        case MealNum3: m_dCurrentPrice = prices.Daves3; break;
        case MealNum4: m_dCurrentPrice = prices.Daves4; break;
    }

    m_dCurrentPrice *= m_iMealQuantity;

    /* Determine additional drink price later */
    switch (m_iDrinkIndex)
    {
        case DrinksNone   : m_dCurrentPrice += prices.noneDrink;    break;
        case DrinksSmall  : m_dCurrentPrice += prices.smallDrink;   break;
        case DrinksRegular: m_dCurrentPrice += prices.regularDrink; break;
        case DrinksLarge  : m_dCurrentPrice += prices.largeDrink;   break;
    }

    return m_dCurrentPrice;
}

//======================================================================================

/*
 * Summary   : This function will dynamically change the heat level text,
 *             based on the position of the heat level QSlider
 * Parameters: int pos - The current position of the heat level QSlider
 * Return    : void
 */
void RestaurantMenuWindow::ChangeHeatLevel(int pos)
{
    switch (pos)
    {
        case 0: ui->heatDynamicLabel->setText("No Spice");  break;
        case 1: ui->heatDynamicLabel->setText("Lite Mild"); break;
        case 2: ui->heatDynamicLabel->setText("Mild");      break;
        case 3: ui->heatDynamicLabel->setText("Medium");    break;
        case 4: ui->heatDynamicLabel->setText("Hot");       break;
        case 5: ui->heatDynamicLabel->setText("Extra Hot"); break;
        case 6: ui->heatDynamicLabel->setText("Reaper");    break;
    }
}

//======================================================================================

/*
 * Summary   : This function will create a QProgressDialog to display the following:
 *             - Your time remaining for waiting purposes
 *             - Your food's status
 * Parameters: int totalTime   - Total time required to process
 *             QWidget *parent - For memory management purposes
 * Return    : void
 */
void RestaurantMenuWindow::ShowTimerProgressDlg(int totalTime, QWidget *parent)
{
    QProgressDialog *progDlg = new QProgressDialog("Time Remaining", "Cancel", 0, totalTime, parent);
    progDlg->setWindowModality(Qt::ApplicationModal);
    progDlg->setMinimumDuration(0);

    QProgressBar *bar = progDlg->findChild<QProgressBar*>();
    if (bar != nullptr) {
        bar->setTextVisible(false);
    }

    QTimer *timer = new QTimer(progDlg);
    int remaining = totalTime;
    QString progLabel;

    progDlg->show();

    QObject::connect(timer, &QTimer::timeout, progDlg, [=] () mutable {
        progDlg->setValue(totalTime - (remaining--));

        progLabel = QString("Time Remaining: %1 minutes and %2 seconds")
            .arg(remaining / 60)
            .arg(remaining % 60);

        progLabel += FindCurrentProcess(progDlg->value(), totalTime);

        progDlg->setLabelText(progLabel);

        if (progDlg->wasCanceled())
        {
            QString endMsg = "You've cancelled your order. Refund issued.";
            QMessageBox::warning(parent, "Signal Interruption", endMsg);
            timer->stop();
            progDlg->close();
        }
        else if (remaining <= 0)
        {
            QString formattedTime = QString("%1 minutes and %2 seconds")
                .arg(totalTime / 60)
                .arg(totalTime % 60);

            QString endMsg = "Your order is ready:\n"
                + ui->mealsComboBox->currentText() + "\n"
                + ui->drinksComboBox->currentText() + "\n"
                + "Heat Level - " + ui->heatDynamicLabel->text() + "\n"
                + "Amount Paid: $" + QString::number(ui->priceCounter->value()) + "\n"
                + "\nTime Lapse: " + formattedTime;

            QMessageBox::information(parent, "Receipt", endMsg);
            timer->stop();
            progDlg->close();
            parent->close(); // Return to start screen after seeing your receipt
        }
    });

    timer->start(1000); // Update every 1000ms or 1s
}

//======================================================================================

/*
 * Summary   : This function will take currentTime and determine your current status.
 * Parameters: int currentTime - Self-explanatory
 *             int totalTime   - Self-explanatory
 * Return    : QString object to add onto progLabel
 */
QString RestaurantMenuWindow::FindCurrentProcess(int currentTime, int totalTime)
{
    // Allocate first 10s for payment processing
    if (currentTime < 10) {
        return QString("\nProcessing Payment...");
    }

    // Whatever time b/w 10s and 90% of totalTime is for cooking
    else if (currentTime >= 10 && currentTime < totalTime - 10) {
        return QString("\nPreparing Meal...");
    }

    // Allocate final 10s to prepare serving
    else {
        return QString("\nFinalizing Preparations...");
    }
}

//======================================================================================
