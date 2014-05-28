#ifndef BARGRAPH_H
#define BARGRAPH_H

#include <QWidget>

class BarGraph : public QWidget
{
    Q_OBJECT
    std::vector<int> values;
    std::vector<QString> label;
    int max_val,min_val,barwidth,margin = 10;
    std::vector<QRect> bars;
    bool show_tooltip=false,show_label=true,cut_min_value=false;
    QBrush brush;
    QString ylabel;
public:
    explicit BarGraph(QWidget *parent = 0);
    void setData(std::vector<int> values,std::vector<QString> labels=std::vector<QString>());
    void setLabel(std::vector<QString> labels){ label = std::move(labels);}
    void paintBargraph(QPainter& painter);

    void setShow_tooltip(bool value);

    bool getShow_label() const;
    void setShow_label(bool value);

    QBrush getBrush() const;
    void setBrush(const QBrush &value);

    bool getCut_min_value() const;
    void setCut_min_value(bool value);

    int getMargin() const;
    void setMargin(int value);

    QString getYlabel() const;
    void setYlabel(const QString &value);

signals:
    
public slots:
protected:

private:
    void paintEvent(QPaintEvent *event)override;
    void mouseMoveEvent(QMouseEvent *event)override;
    void resizeEvent(QResizeEvent *event)override;
    void recalcBasicValues();
    QString getLabel(size_t i);
};

#endif // BARGRAPH_H
