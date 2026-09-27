// MARK: - HyperLang Standard Library: Basic (Swift Runtime)

public enum Basic {
    
    public static let VERSION: String = "1.0.0"
    
    /// Prints a message followed by a newline.
    public static func print(_ message: String) {
        Swift.print(message)
    }
    
    /// Prints a message without a trailing newline.
    public static func write(_ message: String) {
        Swift.print(message, terminator: "")
    }
    
    /// Reads a line from standard input.
    public static func readLine() -> String {
        return Swift.readLine() ?? ""
    }
    
    /// Converts an integer value to a String.
    public static func intToString(_ value: Int) -> String {
        return String(value)
    }
    
    /// Converts a String to an Integer, returning nil or throwing on failure.
    public static func stringToInt(_ value: String) throws -> Int {
        guard let intVal = Int(value) else {
            struct ConversionError: Error {}
            throw ConversionError()
        }
        return intVal
    }
    
    /// Exits the process with a status code.
    public static func exit(code: Int) -> Never {
        exit(Int32(code))
    }
}
