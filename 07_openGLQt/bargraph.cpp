#include "bargraph.h"

#include <QPainter>
#include <QFontMetrics>
#include <QMouseEvent>
#include <QRect>
#include <QTextItem>


void BarGraph::setShow_tooltip(bool value)
{
    show_tooltip = value;
    if(!value)
        setToolTip("");
    setMouseTracking(value);
}

bool BarGraph::getShow_label() const
{
    return show_label;
}

void BarGraph::setShow_label(bool value)
{
    show_label = value;
}


QBrush BarGraph::getBrush() const
{
    return brush;
}

void BarGraph::setBrush(const QBrush &value)
{
    brush = value;
}

bool BarGraph::getCut_min_value() const
{
    return cut_min_value;
}

void BarGraph::setCut_min_value(bool value)
{
    cut_min_value = value;
}

int BarGraph::getMargin() const
{
    return margin;
}

void BarGraph::setMargin(int value)
{
    margin = value;
}

QString BarGraph::getYlabel() const
{
    return ylabel;
}

void BarGraph::setYlabel(const QString &value)
{
    ylabel = value;
}
BarGraph::BarGraph(QWidget *parent) :
    QWidget(parent),brush(Qt::blue)
{

}

void BarGraph::setData(std::vector<int> val,std::vector<QString> labels)
{
    values =std::move(val);
    label = std::move(labels);
    min_val = *std::min_element(values.begin(),values.end());
    if(cut_min_value)
    {
        for(auto& val:values)
            val -= min_val;
    }
    max_val =* std::max_element(values.begin(),values.end());
    recalcBasicValues();
    setMinimumSize( (int)(margin*values.size()*2),max_val+margin*5);
}

void BarGraph::paintBargraph(QPainter &painter)
{
    QPen p(Qt::black);
    p.setWidth(2);
    painter.setPen(p);
    painter.setBrush(brush);

    int y = height() - margin* 2;
    QFontMetrics fm(painter.font());
    int x_lbl = margin+barwidth/2;
    for(size_t i=0, s = values.size(); i < s; ++i)
    {
        painter.drawRect(bars[i]);
        painter.rotate(90);
        if(show_label)
            painter.drawText(x_lbl-fm.width(getLabel(i))/2,y,getLabel(i));

        int min = cut_min_value ? min_val : 0;
        QString lbl = "("+QString::number(values[i]+min)+")";
        painter.drawText(x_lbl-fm.width(lbl)/2,y+fm.height(),lbl);
        x_lbl += margin+barwidth;
        painter.rotate(-90);
    }

    //if(!ylabel.isEmpty())
    {//-fm.width(ylabel)/2

        painter.drawText(height()/2,width()/2,"ylabel"+ylabel);
    }
}

void BarGraph::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    paintBargraph(painter);
}

void BarGraph::mouseMoveEvent(QMouseEvent *event)
{
    if(!show_tooltip)
        return;
    QPoint pos = event->pos();
    auto it = std::find_if(bars.begin(),bars.end(),[pos](const QRect& r){return r.contains(pos);});
    if(it != bars.end())
    {
        auto i = std::distance(bars.begin(),it);
        setToolTip(getLabel(i));
    }
    else if(!toolTip().isEmpty())
        setToolTip("");
}

void BarGraph::resizeEvent(QResizeEvent *event)
{
    recalcBasicValues();
}

void BarGraph::recalcBasicValues()
{
    barwidth = std::max(margin, (int)((width()-(margin*values.size()))/values.size()));

    int h = height()- (margin * 4);
    double factor = ((double)h) /max_val;
    if(min_val < 0)
        h -= min_val;
    if(bars.size() != values.size())
        bars.resize(values.size());
    int x = margin;
    for(size_t i=0, s = values.size(); i < s; ++i)
    {
        double barheight = values[i]*factor;
        bars[i].setRect(x,h -barheight+margin,barwidth, barheight);
        x += margin + barwidth;
    }
}

QString BarGraph::getLabel(size_t i)
{
    if(label.size() > i)
        return label[i];
    if(values.size() > i)
    {
        if(cut_min_value)
            return QString::number(values[i]+min_val);
        return QString::number(values[i]);
    }
    return QString::number(i);
}
