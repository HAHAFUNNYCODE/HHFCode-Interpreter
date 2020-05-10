
/** @namespace Lexer 
 * @brief A namespace for Lexer components.
 * @details A Namespace containing the components needed for the Lexer to function.
 *  This includes structures for holding Lexer data and functions to initialize
 *  the data. It also hold Subclasses of the Lexer base class for switching ability.
 * 
*/

//******
//Lexeme
//******

/** @fn Lexer::Lexeme::Lexeme()
 * @brief A constructor.
 * @details The default constructor that sets the type to UNKNOWN and Line/Column to -1.
*/

/** @fn Lexer::Lexeme::Lexeme(Token)
 * @brief A constructor.
 * @details Constructs a Lexeme with a specific type with default value and line/column.
 * @param type The type to use for the Lexeme.
*/

/** @fn Lexer::Lexeme::Lexeme(Token, std::string, int, int)
 * @brief A constructor.
 * @details A full constructor, specifying all Lexeme data.
 * @param type The type for the lexeme.
 * @param value The content of the lexeme.
 * @param line, column The position of the lexeme in the file.
*/

/** @fn Lexer::Lexeme::equals
 * @brief Checks equality between two lexemes
 * @details Checks for the equality between two lexemes.
 *  Lexemes are deemed equal if both type and value are the same.
 * 
 * @param other The lexeme to which to compare to.
 * @returns true if equal, false if not equal.
 * @see Lexer::Lexeme::operator==()
*/

/** @fn Lexer::Lexeme::operator==()
 * @brief Checks equality between two lexemes
 * @details Checks for the equality between two lexemes.
 *  Lexemes are deemed equal if both type and value are the same.
 * @param other The lexeme to which to compare to.
 * @returns true if equal, false if not equal.
 * @see Lexer::Lexeme::equals
 * @see Lexer::Lexeme::operator!=()
*/

/** @fn Lexer::Lexeme::operator!=()
 * @brief Checks inequality between two lexemes
 * @details Checks for the equality between two lexemes.
 *  Lexemes are deemed equal if both type and value are the same.
 * @param other The lexeme to which to compare to.
 * @returns false if equal, true if not equal.
 * @see Lexer::Lexeme::equals
 * @see Lexer::Lexeme::operator==()
*/

/** @fn Lexer::Lexeme::InvalidTokenException::InvalidTokenException(std::string) 
 * @brief A constructor.
 * @details Constructor that allows for a custom message.
 * @param msg The message that the exception will give out. */

/** @fn Lexer::Lexeme::InvalidTokenException::InvalidTokenException(const InvalidTokenException&) 
 * @brief A constructor. 
 * @details Copy constructor for the exception.
 * @param e A reference to an existing exception. */

//******
//Lexer
//******

/** @class Lexer::Lexer::LexerUninitializedException
 * @details Thrown by the Lexer::Lexer when a function tries to use it without it being initialized.
 * */

/** @fn Lexer::Lexer::initialize
 * @brief Initializes the lexer.
 * @details By default, this function does nothing. It is an optional virtual overload
 *  that allows for the a subclass of Lexer to initialize outside of the constructor.
 * */

/** @fn Lexer::Lexer::loadPatterns
 * @brief Loads patterns that have been initialized.
 * @details The patterns that are made in the constructor need to be loaded into Tries for the
 *  base class tokenize function to work. This function takes all of the relavent data and puts
 *  it into a Trie data structure.
 * */

/** @fn Lexer::Lexer::tokenize
 * @brief Performs the lexical analysis.
 * @details The function takes a string as data and looks through to find all of the Lexemes that
 *  are inside of the data. It does this in linear time.
 * @param input The string that will be analyzed. This input will likely come from a file, although
 *  any string works.
 * @returns A LexemeStream containing all of the lexemes that were found in order.
 * @throw LexerUninitializedException is thrown if the lexer has not yet been initialized.
 * @throw Lexeme::InvalidTokenException is thrown if the lexer encounters something that
 *  it cannot understand or there is a syntax error.
 * */

/**
 * @fn Lexer::Lexer::getLiteral
 * @details Depending on the starting character, it attempts to get either a string, number,
 *  or some keyword literal.
 * 
 * @param input A reference to the input data string.
 * @param index A pointer to the index keeping track of file position.
 * @returns A LITERAL lexeme, UNKNOWN lexeme, or INVALID Lexeme, depending on what the wrapped
 *  functions return.
 * */

/**
 * @fn Lexer::Lexer::getIdentifier
 * @details Starting from a letter or underscore (_), it looks until it finds an
 *  operator, separator or whitespace, or a non-identifier character.
 * 
 * @param input A reference to the input data string.
 * @param index A pointer to the index keeping track of file position.
 * @returns A IDENTIFIER lexeme if it finds a valid IDENTIFER or an UNKNOWN lexeme.
 * */

/**
 * @var Lexer::Lexer::knownSymbols
 * @brief The symbols that the lexer can match to.
 * @details These symbols are used by loadPatterns to make the pattern Tries. The keys should
 *  be the Lexeme::Token and the values that the token matches to.
 * */

/**
 * @var Lexer::Lexer::startChars
 * @brief Contains all of chars that each Token's patterns start with.
 * @details The keys are Lexeme::Token and the values are a hashset containing all chars that
 *  the patterns in knownSymbols start with under the same key.
 * */

/**
 * @var Lexer::Lexer::whitespace
 * @brief A set of whitespace characters.
 * @details A hashset of characters representing everything that is considered whitespace.
 * */

/**
 * @var Lexer::Lexer::identifierSet
 * @brief A set of identifier characters.
 * @details A hashset of all characters that are valid for having in identifiers.
 * */

//************
//LexemeStream
//************

/**
 * @fn Lexer::LexemeStream::pushLexeme
 * @details Checks to see if the stream is open, if it is, it adds the lexeme to the 
 *  queue of lexeme.
 * @param lex The lexeme that is added if the stream is open
 * @throw ClosedLexStreamException if the stream is not open
 * */

/**
 * @fn Lexer::LexemeStream::finish
 * @details Will attempt to push a FILEEND lexeme (EOF) and close the stream.
 * @throw ClosedLexStreamException if the stream is already closed.
 * */

/**
 * @fn Lexer::LexemeStream::next
 * @details Tries to grab the next Lexeme in the queue. Will fail if either the stream
 *  is done or the queue is empty. If it is the last lexeme in the queue, it will
 *  set the stream to done.
 * @returns The next lexeme in the queue.
 * @throw FinishedLexStreamException
 * */

/**
 * @fn Lexer::LexemeStream::isOpen
 * @details Returns whether the stream is open to pushing or not.
 * @returns The open status of the stream.
 * */

/**
 * @fn Lexer::LexemeStream::isDone
 * @details Returns whether the stream is done it is still possible to get a value.
 * @returns The done status of the stream.
 * */

/**
 * @fn Lexer::LexemeStream::ClosedLexStreamException
 * @details An exception to be thrown if attempting to push to a closed stream.
 * */

/**
 * @fn Lexer::LexemeStream::FinishedLexStreamException
 * @details An exception to be thrown if attempting to get a lexeme from an empty
 *  or finished stream.
 * */

//******
//Trie
//******

/** 
 * @fn Lexer::Trie::addPattern(std::string p)
 * @details Recursive adds characters from the pattern to nodes in the trie. At the
 *  last node, it sets the "ending" flag to be true.
 * @param p The string to add as a pattern
 * @see isEnding
 * */

/**
 * @fn Lexer::Trie::getRef
 * @details Tries to find a reference to the requested node. Will simply return the
 *  pointer to it, which may be nullptr if it does not exist.
 * @param c The character node which it tries to lookup
 * */

/**
 * @fn Lexer::Trie::isEnding
 * @details The result will be the flag that would be set when adding patterns.
 * @see addPattern(std::string)
 * */


//************
//IndexTracker
//************

/**
 * @fn Lexer::IndexTracker::addLine
 * @details Goes to next line and starts at column 1.
 * */

/**
 * @fn Lexer::IndexTracker::operator size_t()
 * @details Simply returns the index which is already a size_t.
 *  Made for implicit conversions to size_t.
 * */