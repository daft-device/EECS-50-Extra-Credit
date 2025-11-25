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

    /* Create function connections */
    connect(ui->orderButton, &QPushButton::clicked, this,
        &RestaurantMenuWindow::PlaceOrderBtnClicked);

    connect(ui->mealsComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [&] () {
            double price = CalculateCurrentPrice();
            ui->priceCounter->display(price);
        });

    connect(ui->drinksComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [&] () {
            double price = CalculateCurrentPrice();
            ui->priceCounter->display(price);
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
    QProgressDialog* ProgDlg = new QProgressDialog(this);
    ProgDlg->setWindowModality(Qt::ApplicationModal);
    ProgDlg->setWindowTitle("Kitchen");
    ProgDlg->setLabelText("Processing Payment...");

    int steps = 512;
    ProgDlg->setRange(0, steps);

    for (int i = 0; i < steps; i++)
    {
        // Process trigger of "Cancel" button
        if (ProgDlg->wasCanceled())
        {
            QString interruptMsg = "You've cancelled your order. Refund issued.";
            QMessageBox::information(this, "Signal Interruption", interruptMsg);

            ProgDlg->hide();
            delete ProgDlg;
            ProgDlg = nullptr;
            return;
        }

        // 25% completion
        if (i == (steps / 4)) {
            ProgDlg->setLabelText("Preparing Meal...");
        }

        // 80% completion
        else if (i == (steps / 5) * 4) {
            ProgDlg->setLabelText("Finishing Preparations...");
        }

        ProgDlg->setValue(i);
        QThread::msleep(50); // Delay 50ms per step
    }

    ProgDlg->setValue(steps); // Ending the progess bar

    ProgDlg->hide();
    delete ProgDlg;
    ProgDlg = nullptr;

    int totalTime = 50 * 512; // 50ms * 512 steps
    QString endMsg = "Your order is ready:\n" + ui->mealsComboBox->currentText()
                     + "\n" + ui->drinksComboBox->currentText()
                     + "\n\nTime Lapse (ms): " + QString::number(totalTime);
    QMessageBox::information(this, "Receipt", endMsg);
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

    /* Determine meal price first */
    switch (m_iMealIndex)
    {
        case MealNum1: m_dCurrentPrice = prices.Daves1; break;
        case MealNum2: m_dCurrentPrice = prices.Daves2; break;
        case MealNum3: m_dCurrentPrice = prices.Daves3; break;
        case MealNum4: m_dCurrentPrice = prices.Daves4; break;
    }

    /* Determine additional drink price later */
    switch (m_iDrinkIndex)
    {
        case DrinksNone:    m_dCurrentPrice += prices.noneDrink;    break;
        case DrinksSmall:   m_dCurrentPrice += prices.smallDrink;   break;
        case DrinksRegular: m_dCurrentPrice += prices.regularDrink; break;
        case DrinksLarge:   m_dCurrentPrice += prices.largeDrink;   break;
    }

    return m_dCurrentPrice;
}

//======================================================================================
