/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright © 2010-2015 FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#ifndef FL_SNORMFACTORY_H
#define FL_SNORMFACTORY_H

#include "../factory/ConstructionFactory.h"

#include "../norm/SNorm.h"

namespace fl {

    class FL_API SNormFactory : public ConstructionFactory<SNorm*> {
    public:
        SNormFactory();
        virtual ~SNormFactory() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(SNormFactory)
    };
}
#endif  /* FL_SNORMFACTORY_H */

