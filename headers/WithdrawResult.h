//
// Created by adi on 12/20/25.
//

#pragma once
enum class WithdrawResult {
    Success = 0,
    ATMError = 1,
    //InvalidAmount = 2,
    AuthError = 3,
    InsufficientFunds=4,      
    DailyLimitExceeded=5,     
    MonthlyLimitExceeded=6,   
    CardLimitExceeded=7,    
    AmountNotDivisible=8,     
    AmountTooLow=9
};
