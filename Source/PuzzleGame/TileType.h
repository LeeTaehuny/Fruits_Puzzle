#pragma once

UENUM(BlueprintType)
enum class ETileType : uint8
{
    TT_Apple,       // 0 (사과)
    TT_Banana,      // 1 (바나나)
    TT_PineApple,   // 2 (파인애플)
    TT_Cherry,      // 3 (체리)
    TT_MaxNum,      // 4 (타일의 총 가지 수)

    TT_Bomb,    // 3 (폭탄 - 광역)
};