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

#ifndef FL_TERMFACTORY_H
#define FL_TERMFACTORY_H


#include "../factory/ConstructionFactory.h"

#include "../term/Term.h"

namespace fl {

    class FL_API TermFactory : public ConstructionFactory<Term*> {
    public:
        TermFactory();
        virtual ~TermFactory() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(TermFactory)
    };
}
#endif  /* FL_TERMFACTORY_H */

