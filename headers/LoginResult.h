//
// Created by adi on 12/20/25.
//

#pragma once
enum class LoginResult {
    Success = 0,
    Blocked = 1,
    InvalidCardNumber = 2,
    InvalidPin = 3,
    AccountError = 4
};
