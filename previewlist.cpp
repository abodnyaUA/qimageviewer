#include "previewlist.h"

previewList::previewList(QStringList list)
{
    for (int i=0;i<list.size();i++)
    {
        preview * temp = new preview(list[i],100);
        lst.append(temp);
        layout.addWidget(temp);
    }
    setLayout(&layout);
}
