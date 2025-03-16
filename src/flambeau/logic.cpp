/**
 *  logic.cpp
 *  
 *  Created on: Oct 27, 2012
 *      Author: nav
 *  
 */

#include "flambeau/logic.h"

namespace Flambeau
{

const unsigned int Logic::Count;

const Logic Logic::L(0);
const Logic Logic::H(1);
const Logic Logic::Z(2);
const Logic Logic::X(3);

const Logic Logic::AndLookup[Logic::Count][Logic::Count] = 
{
    {   Logic::L,   Logic::L,   Logic::X,   Logic::X    },
    {   Logic::L,   Logic::H,   Logic::X,   Logic::X    },
    {   Logic::X,   Logic::X,   Logic::X,   Logic::X    },
    {   Logic::X,   Logic::X,   Logic::X,   Logic::X    }
};

const Logic Logic::OrLookup[Logic::Count][Logic::Count] = 
{
    {   Logic::L,   Logic::H,   Logic::X,   Logic::X    },
    {   Logic::H,   Logic::H,   Logic::X,   Logic::X    },
    {   Logic::X,   Logic::X,   Logic::X,   Logic::X    },
    {   Logic::X,   Logic::X,   Logic::X,   Logic::X    }
};

const Logic Logic::XorLookup[Logic::Count][Logic::Count] = 
{
    {   Logic::L,   Logic::H,   Logic::X,   Logic::X    },
    {   Logic::H,   Logic::L,   Logic::X,   Logic::X    },
    {   Logic::X,   Logic::X,   Logic::X,   Logic::X    },
    {   Logic::X,   Logic::X,   Logic::X,   Logic::X    }
};

const Logic Logic::NotLookup[Logic::Count] =
{
    Logic::H,
    Logic::L,
    Logic::X,
    Logic::X
};

const Logic Logic::TriLookup[Logic::Count][Logic::Count] = 
{
    {   Logic::Z,   Logic::L,   Logic::X,   Logic::X    },
    {   Logic::Z,   Logic::H,   Logic::X,   Logic::X    },
    {   Logic::Z,   Logic::X,   Logic::X,   Logic::X    },
    {   Logic::Z,   Logic::X,   Logic::X,   Logic::X    }
};

const Logic Logic::DrvLookup[Logic::Count][Logic::Count] = 
{
    {   Logic::X,   Logic::X,   Logic::L,   Logic::X    },
    {   Logic::X,   Logic::X,   Logic::H,   Logic::X    },
    {   Logic::L,   Logic::H,   Logic::Z,   Logic::X    },
    {   Logic::X,   Logic::X,   Logic::X,   Logic::X    }
};

const char Logic::StrLookup[Logic::Count] =
{
    '0',
    '1',
    'Z',
    'X'
};

Logic::Logic(uint8_t value)
    : Value(value)
{
    
}

Logic::Logic()
    : Value(Logic::X.Value)
{
    
}

Logic::Logic(const Logic & a)
    : Value(a.Value)
{
    
}

Logic::~Logic()
{
    
}

}   // namespace Flambeau
