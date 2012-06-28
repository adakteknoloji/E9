#ifndef HTML_TAG_H
#define HTML_TAG_H

#include <QString>

class HTML_TAG
{
public:
    HTML_TAG();
    virtual ~HTML_TAG();

protected:
    void               SET_VALUE                 (const QString value);
    void               SET_TAG_NAME              (const QString tag_name);
    void               SET_ATTRIBUTE             (const QString attribute_name,QString attribute_value );
    QString            GET_HTML_TAG              ();

private:
    QString            p_tag_name;
    QString            p_value;
    QString            p_attribute_name;
    QString            p_attribute_value;
};

#endif // HTML_TAG_H
