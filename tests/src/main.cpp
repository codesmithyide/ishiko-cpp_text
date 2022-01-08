/*
    Copyright (c) 2020-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/text/blob/main/LICENSE.txt
*/

#include "ASCIITests.hpp"
#include "CStringTests.hpp"
#include <Ishiko/Tests/Core.hpp>
#include <Ishiko/Text/linkoptions.hpp>

using namespace Ishiko::Tests;

int main(int argc, char* argv[])
{
    TestHarness theTestHarness("IshikoText");

    TestSequence& theTests = theTestHarness.tests();
    theTests.append<ASCIITests>();
    theTests.append<CStringTests>();

    return theTestHarness.run();
}
