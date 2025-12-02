# Project Templates

This folder contains templates to help you quickly start new Arduino projects with a consistent structure.

## Using the Template

### Method 1: Copy the Template Folder
1. Copy the `project-template` folder
2. Rename it to your project name (e.g., `my-awesome-project`)
3. Move it to the appropriate category folder in `projects/`
4. Rename `project-name.ino` to match your project name
5. Fill in the template with your project details

### Method 2: Use the Template as Reference
1. Create a new folder in the appropriate `projects/` category
2. Create your `.ino` sketch file
3. Create a README.md file
4. Use the template as a guide for what to include

## Template Structure

```
project-template/
├── project-name.ino    # Arduino sketch with code template
└── README.md           # Project documentation template
```

## What to Include in Your Project

### Arduino Sketch (.ino file)
Your `.ino` file should include:
- Header comment block with:
  - Project name and description
  - Author and date
  - Hardware requirements
  - Pin connections
  - Required libraries
- Library includes
- Pin definitions with clear names
- Global variables
- `setup()` function
- `loop()` function
- Additional helper functions

### README.md File
Your README should include:
- Project title and brief description
- Detailed description of what the project does
- List of required components
- Circuit diagram or wiring instructions
- Required libraries and how to install them
- Code explanation
- How to use the project
- Expected output or behavior
- Troubleshooting tips
- Possible improvements or variations
- Photos or videos (optional but recommended)
- What you learned
- References and resources

## Tips for Good Documentation

### Be Clear and Concise
- Write as if explaining to someone new to Arduino
- Use simple language
- Break complex concepts into steps

### Include Visual Aids
- Circuit diagrams (Fritzing, ASCII art, or photos)
- Photos of the completed project
- Videos or GIFs of the project in action
- Screenshots of serial output

### Explain Your Code
- Add comments to complex sections
- Explain why you made certain decisions
- Document any tricks or workarounds used

### Test Your Instructions
- Follow your own wiring instructions
- Verify your component list is complete
- Check that library names are correct
- Test that code compiles and works

## Example Projects

See `projects/01-basics/blink-led/` for an example of a well-documented project using this template structure.

## File Naming Conventions

### Sketch Files
- Use lowercase with hyphens: `my-project.ino`
- Match the folder name
- Be descriptive: `ultrasonic-distance-sensor.ino` not `project1.ino`

### Folders
- Use lowercase with hyphens
- Be descriptive but concise
- Examples:
  - ✅ `temperature-display`
  - ✅ `servo-sweep`
  - ✅ `bluetooth-car`
  - ❌ `temp` (too short)
  - ❌ `MyAwesomeProject` (use lowercase)
  - ❌ `test1` (not descriptive)

### Image Files
- Store in project folder or `photos/` subfolder
- Use descriptive names: `circuit-diagram.png`, `working-project.jpg`
- Keep file sizes reasonable (compress if needed)

## Required vs. Optional Elements

### Required
- ✅ Arduino sketch file (.ino)
- ✅ README.md with basic documentation
- ✅ Component list
- ✅ Basic wiring/connection information

### Recommended
- ⭐ Circuit diagram
- ⭐ Code comments and explanation
- ⭐ Photos of working project
- ⭐ Troubleshooting section

### Optional
- ⚪ Multiple code examples or variations
- ⚪ Datasheets or additional documentation
- ⚪ Videos or animated GIFs
- ⚪ Schematic files (Fritzing, KiCad)
- ⚪ PCB design files

## Keeping Projects Organized

### Folder Structure
```
projects/
└── category/
    └── project-name/
        ├── project-name.ino
        ├── README.md
        ├── photos/              (optional)
        │   ├── circuit.jpg
        │   └── working.jpg
        ├── schematics/          (optional)
        │   └── circuit.fzz
        └── datasheets/          (optional)
            └── sensor-datasheet.pdf
```

### Version Control
- Commit working versions
- Write clear commit messages
- Tag major milestones
- Keep a changelog for complex projects

### Code Organization
- Use functions for repeated code
- Group related variables together
- Add section comments for clarity
- Keep functions focused on single tasks

## Common Mistakes to Avoid

❌ **Don't**:
- Leave template placeholder text unchanged
- Skip the component list (you'll forget what you used)
- Forget to test your wiring instructions
- Use unclear variable names
- Skip comments in complex code

✅ **Do**:
- Fill in all relevant template sections
- Test your project before documenting
- Include actual pin numbers in wiring instructions
- Use descriptive names for everything
- Update documentation as you make changes

## Getting Help

If you're unsure about:
- How to document a complex circuit → Check other projects for examples
- What level of detail to include → More is usually better
- How to explain your code → Imagine teaching a friend
- Circuit diagram tools → See resources/README.md for tools list

## Contributing Your Projects

While this is a personal learning repository, well-documented projects can:
- Help you remember what you learned
- Serve as reference for future projects
- Help others learning Arduino
- Build your portfolio
- Track your progress over time

Remember: Every expert was once a beginner. Document your learning journey!
