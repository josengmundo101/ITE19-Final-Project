#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function Prototypes
int getValue(char roman);
int romanToDecimal(const char *roman);
void decimalToRoman(int num, char *result);
void numberToWords(int num, char *result);
void processFile(const char *input, const char *output);
void displayResult(const char *operand1, int decimal1, const char *operand2, int decimal2, char operator, int result, const char *wordsResult);


int main() {
    processFile("input.txt", "output.txt");  // Process input file and generate output
    printf("Conversion complete. Check output.txt for results.\n");
    return 0;
}

// Function to get the decimal value of a Roman numeral character
int getValue(char roman) {
    switch (roman) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return 0;
    }
}

// Function to convert Roman numeral string to decimal
int romanToDecimal(const char *roman) {
    int total = 0;
    int prevValue = 0;

    for (int i = 0; roman[i] != '\0'; i++) {
        int currentValue = getValue(roman[i]);

        if (currentValue == 0) return -1;  // Invalid Roman numeral

        if (currentValue > prevValue) {
            total += currentValue - 2 * prevValue;  // Correct for subtractive notation
        } else {
            total += currentValue;
        }

        prevValue = currentValue;
    }

    return total;
}

// Function to convert decimal to Roman numeral
void decimalToRoman(int num, char *result) {
    const struct {
        int value;
        const char *symbol;
    } roman[] = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
        {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"},
        {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}
    };

    result[0] = '\0';  // Initialize as empty string

    for (int i = 0; i < 13 && num > 0; i++) {
        while (num >= roman[i].value) {
            strcat(result, roman[i].symbol);
            num -= roman[i].value;
        }
    }
}

// Function to convert decimal to English words
void numberToWords(int num, char *result) {
    const char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    const char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    const char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    const char *thousands[] = {"", "Thousand", "Million", "Billion"};

    char temp[200] = "";
    int partCount = 0;

    while (num > 0) {
        int part = num % 1000;

        if (part > 0) {
            char partResult[200] = "";

            if (part / 100 > 0) {
                sprintf(partResult, "%s Hundred ", ones[part / 100]);
            }

            if (part % 100 >= 10 && part % 100 < 20) {
                strcat(partResult, teens[part % 100 - 10]);
            } else {
                strcat(partResult, tens[(part % 100) / 10]);
                strcat(partResult, " ");
                strcat(partResult, ones[part % 10]);
            }

            if (strlen(thousands[partCount]) > 0) {
                strcat(partResult, " ");
                strcat(partResult, thousands[partCount]);
            }

            strcat(partResult, " ");
            strcat(partResult, temp);
            strcpy(temp, partResult);
        }

        num /= 1000;
        partCount++;
    }

    strcpy(result, temp);
}

// Function to process input and output files
void processFile(const char *input, const char *output) {
    FILE *inputFile = fopen(input, "r");
    FILE *outputFile = fopen(output, "w");

    if (inputFile == NULL || outputFile == NULL) {
        printf("Error opening files.\n");
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), inputFile)) {
        char operand1[100], operand2[100], wordsResult[200];
        char operator;
        int decimal1, decimal2, result;

        // Parse the line
        if (sscanf(line, "%s %c %s", operand1, &operator, operand2) == 3) {
            decimal1 = romanToDecimal(operand1);
            decimal2 = romanToDecimal(operand2);

            if (decimal1 == -1 || decimal2 == -1) {
                fprintf(outputFile, "Invalid Roman numeral: %s\n", line);
                continue;
            }

            // Perform operation
            switch (operator) {
                case '+': result = decimal1 + decimal2; break;
                case '-': result = decimal1 - decimal2; break;
                case '*': result = decimal1 * decimal2; break;
                case '/':
                    if (decimal2 == 0) {
                        fprintf(outputFile, "Error: Division by zero: %s\n", line);
                        continue;
                    }
                    result = decimal1 / decimal2;
                    break;
                default:
                    fprintf(outputFile, "Invalid operator: %s\n", line);
                    continue;
            }

            // Convert result to words
            numberToWords(result, wordsResult);
            fprintf(outputFile, "%s\n", wordsResult);

            // Display to console
            displayResult(operand1, decimal1, operand2, decimal2, operator, result, wordsResult);
        } else {
            fprintf(outputFile, "Invalid input format: %s\n", line);
        }
    }

    fclose(inputFile);
    fclose(outputFile);
}

// Function to display results on console
void displayResult(const char *operand1, int decimal1, const char *operand2, int decimal2, char operator, int result, const char *wordsResult) {
    printf("%s (%d) %c %s (%d) = %d (%s)\n", operand1, decimal1, operator, operand2, decimal2, result, wordsResult);
}
