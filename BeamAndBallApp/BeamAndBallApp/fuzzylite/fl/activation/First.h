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

#ifndef FL_FIRST_H
#define	FL_FIRST_H

#include "../activation/Activation.h"

namespace fl {

    class FL_API First : public Activation {
    public:

        First();
        virtual ~First();
        FL_DEFAULT_COPY_AND_MOVE(First)

        virtual std::string className() const FL_IOVERRIDE;

        virtual std::string parameters() const FL_IOVERRIDE;
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual void activate(RuleBlock* ruleBlock) const FL_IOVERRIDE;

        virtual First* clone() const FL_IOVERRIDE;

        static Activation* constructor();
    };

}


#endif	/* FL_FIRST_H */
