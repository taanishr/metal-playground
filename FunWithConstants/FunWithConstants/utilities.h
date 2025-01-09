//
//  utilities.h
//  FunWithConstants
//
//  Created by Taanish Reja on 1/8/25.
//

constexpr int align(int size, int upTo)
{
    return (size + upTo - 1) & ~(upTo - 1);
}
