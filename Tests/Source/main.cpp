/*
    Copyright (c) 2020 Xavier Leclercq
    Released under the MIT License
    See https://github.com/Ishiko-cpp/Text/blob/master/LICENSE.txt
*/

#include "CStringTests.h"
#include <Ishiko/TestFramework/TestFrameworkCore.h>

using namespace Ishiko::Tests;

int main(int argc, char* argv[])
{
    TestHarness theTestHarness("IshikoText");

    TestSequence& theTests = theTestHarness.tests();
    theTests.append<CStringTests>();

    return theTestHarness.run();
}