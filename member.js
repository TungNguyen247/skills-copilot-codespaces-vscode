// Create a member object for GitHub Skills
const skillsMember = {
    name: "GitHub Copilot",
    role: "AI Pair Programmer",
    skills: [
        "JavaScript",
        "Python",
        "TypeScript",
        "Ruby",
        "Go",
        "C#",
        "C++"
    ],
    greet: function() {
        return `Hello, I'm ${this.name}! I can help you code faster.`;
    }
};

module.exports = skillsMember;
