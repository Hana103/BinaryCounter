#include <QHBoxLayout>
#include "counterwidget.h"
#include "config.h"
#include <QGridLayout>

CounterWidget::CounterWidget(QWidget *parent)
    : QWidget(parent), m_count(0), m_timer(this)
{
    setWindowTitle("BinaryCounter");
    resize(300, 200);
    QHBoxLayout* layout = new QHBoxLayout(this);
    setLayout(layout);

    m_gpio= new Gpio(this);

    // QLCDNumber zur Anzeige des Zaehlerstandes dezimal:
    m_show= new QLCDNumber(this);


    connect(&m_timer, &QTimer::timeout, this, &CounterWidget::updateCounter);
    m_timer.start(T_UPDATE);
}

CounterWidget::~CounterWidget()
{
    delete m_gpio;
}

// Zaehlerstand im Widget und dual ueber LEDs anzeigen:
void CounterWidget::updateCounter()
{
    int n=0;
    for (auto pin : BUTTONS)
    {

        int state = !m_gpio->get(pin);

        if(m_gpio->edgeDetect(state,true,n))
        {
            if(n==0)
            {
                m_count--; // GPIO22-> cnt--
              if(m_count<0)
                {
                m_count=15;
                 }
               m_show->display(m_count);
            }
            else if(n==1)
            {
                m_count=0;    // GPIO27-> cnt=0
             m_show->display(m_count);
            }
            else if(n==2)
            {
                m_count++;    // GPIO17-> cnt++
                if(m_count>15)
                  {
                  m_count=0;
                  }
                 m_show->display(m_count);

            }

            m_show->display(m_count);
        }
        n++;

        m_gpio->set(m_count);

    }
}


