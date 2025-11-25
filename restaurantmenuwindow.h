#ifndef RESTAURANTMENUWINDOW_H
#define RESTAURANTMENUWINDOW_H

#include <QDialog>
#include <QFont>
#include <QDebug>
#include <QProgressDialog>
#include <QThread>
#include <QMessageBox>

namespace Ui {
class RestaurantMenuWindow;
}

//======================================================================================

struct MenuPrices
{
    /* Meal Prices */
    double Daves1 = 12.99;
    double Daves2 = 14.99;
    double Daves3 = 13.99;
    double Daves4 = 10.49;

    /* Drink Prices */
    double noneDrink = 0.0;
    double smallDrink = 2.29;
    double regularDrink = 2.69;
    double largeDrink = 2.99;
};

enum MealIndices
{
    MealNum1 = 0,
    MealNum2,
    MealNum3,
    MealNum4,
};

enum DrinkIndices
{
    DrinksNone = 0,
    DrinksSmall,
    DrinksRegular,
    DrinksLarge,
};

//======================================================================================

class RestaurantMenuWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RestaurantMenuWindow(QWidget *parent = nullptr);
    ~RestaurantMenuWindow();

protected:
    void PlaceOrderBtnClicked();
    double CalculateCurrentPrice();

private:
    Ui::RestaurantMenuWindow *ui;
    double m_dCurrentPrice;
    int m_iMealIndex;
    int m_iDrinkIndex;
    MenuPrices prices;
};

#endif // RESTAURANTMENUWINDOW_H
