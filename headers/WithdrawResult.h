//
// Created by adi on 12/20/25.
//

#pragma once
enum class WithdrawResult {
    Success = 0,
    ATMError = 1,
    InvalidAmount = 2,
    AuthError = 3
};
