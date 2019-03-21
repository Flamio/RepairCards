#ifndef IVIEW_H
#define IVIEW_H

class IView
{
public:
    virtual void showWindow() = 0;
    virtual void closeWindow() = 0;
};

#endif // IVIEW_H
