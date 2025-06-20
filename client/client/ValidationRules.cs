using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Net.Mail;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;

namespace client
{
    public class UsernameValidationRule : ValidationRule
    {
        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            var input = value as string ?? "";
            return Regex.IsMatch(input, @"^[a-zA-Z0-9]{3,20}$")
                ? ValidationResult.ValidResult
                : new ValidationResult(false, "Username must be 3-20 letters and digits.");
        }
    }
    public class ValidateNumbers : ValidationRule
    {
        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            string inputStr = value as string;

            if (string.IsNullOrWhiteSpace(inputStr))
            {
                return new ValidationResult(false, "Input is required");
            }

            if (!int.TryParse(inputStr, out int input))
            {
                return new ValidationResult(false, "Input should be only numbers");
            }

            if (input < 1 || input > 10)
            {
                return new ValidationResult(false, "Input should be between 1 and 10");
            }

            return ValidationResult.ValidResult;
        }
    }
    public class ValidateTimePerQ : ValidationRule
    {
        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            string inputStr = value as string;
            if (string.IsNullOrWhiteSpace(inputStr))
            {
                return new ValidationResult(false, "Input is required");
            }

            if (!int.TryParse(inputStr, out int input))
            {
                return new ValidationResult(false, "Input must be a number");
            }

            if (input < 1 || input > 3600)
            {
                return new ValidationResult(false, "Value must be between 1 and 3600 seconds");
            }

            return ValidationResult.ValidResult;
        }
    }
    public class EmailValidationRule : ValidationRule
    {
        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            var input = value as string ?? "";
            return Regex.IsMatch(input, @"^[^@\s]+@[^@\s]+\.[^@\s]+$")
                ? ValidationResult.ValidResult
                : new ValidationResult(false, "Invalid email format.");
        }
    }
}