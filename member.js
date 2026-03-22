function skillsMember(name, age) {
    return {
        name: name,
        age: age,
        greet: function () {
            return `Hello, my name is ${this.name} and I am ${this.age} years old.`;
        },
    };
}
