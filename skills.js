function calculateNumbers(var1, var2) {
    const sum = var1 + var2;
    const difference = var1 - var2;
    const product = var1 * var2;
    const quotient = var1 / var2;

    console.log(`Sum: ${sum}`);
    console.log(`Difference: ${difference}`);
    console.log(`Product: ${product}`);
    console.log(`Quotient: ${quotient}`);

    return {
        sum,
        difference,
        product,
        quotient
    };
}
