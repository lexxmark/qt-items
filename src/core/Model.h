/*
   Copyright (c) 2008-1015 Alex Zhondin <qtinuum.team@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef QI_MODEL_H
#define QI_MODEL_H

#include "ID.h"

namespace Qi
{

class QI_EXPORT Model: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Model)

protected:
    Model();

public:
    virtual ~Model();
    
signals:
    void modelChanged(const Model*);
};

class QI_EXPORT ModelComparable: public Model
{
    Q_OBJECT
    Q_DISABLE_COPY(ModelComparable)

protected:
    ModelComparable();

public:
    virtual ~ModelComparable();

    int compare(ID left, ID right) const { return compareImpl(left, right); }

    template<typename T>
    int compareAs(T left, T right) const { return compare(ID(left), ID(right)); }

    bool isAscendingDefault(ID item) const { return isAscendingDefaultImpl(item); }

protected:
    virtual int compareImpl(ID left, ID right) const = 0;
    virtual bool isAscendingDefaultImpl(ID /*item*/) const { return true; }
};

} // end namespace Qi

#endif // QI_MODEL_H
