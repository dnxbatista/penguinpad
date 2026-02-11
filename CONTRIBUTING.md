# Contributing to PenguinPad

Thank you for your interest in contributing to PenguinPad! We welcome contributions from the community.

## Code of Conduct

Be respectful, inclusive, and professional in all interactions.

## How to Contribute

### Reporting Bugs
- Use the GitHub issue tracker
- Include steps to reproduce, expected behavior, and actual behavior
- Provide system information (Windows version, MSVC version)

### Suggesting Features
- Open an issue with the "enhancement" label
- Clearly describe the feature and its benefits
- Consider implementation complexity and project scope

### Pull Requests

1. **Fork the repository** and create your branch from `main`
   ```bash
   git checkout -b feature/amazing-feature
   ```

2. **Make your changes**
   - Follow the existing code style (see `.clang-format`)
   - Add comments for complex logic
   - Test your changes thoroughly

3. **Commit your changes**
   ```bash
   git commit -m "Add: Brief description of your change"
   ```
   Use prefixes: `Add:`, `Fix:`, `Update:`, `Remove:`

4. **Push to your fork**
   ```bash
   git push origin feature/amazing-feature
   ```

5. **Open a Pull Request**
   - Provide a clear description of changes
   - Reference any related issues
   - Wait for review and address feedback

## Development Setup

See the [README.md](README.md) for build instructions.

## Code Style

- Use modern C++20 features appropriately
- Follow RAII principles for resource management
- Use meaningful variable and function names
- Format code with `.clang-format` before committing

## Testing

- Test with multiple gamepad types if possible (Xbox, PlayStation, Nintendo Switch, etc.)
- Verify on Windows 10/11
- Check for memory leaks with appropriate tools

## Questions?

Open an issue or discussion on GitHub if you need clarification.
