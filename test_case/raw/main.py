import argparse
import nltk
from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize


def process_text(input_file, output_file, limit):
    # Read input text
    try:
        with open(input_file, "r", encoding="utf-8") as file:
            text = file.read()
    except FileNotFoundError:
        print(f"Error: Input file '{input_file}' not found.")
        return
    except Exception as e:
        print(f"Error reading input file: {e}")
        return

    # Download necessary NLTK resources if not already downloaded
    try:
        nltk.data.find("tokenizers/punkt")
    except LookupError:
        print("Downloading punkt tokenizer...")
        nltk.download("punkt")

    try:
        nltk.data.find("corpora/stopwords")
    except LookupError:
        print("Downloading stopwords...")
        nltk.download("stopwords")

    # Tokenize text
    word_tokens = word_tokenize(text)

    # Get stopwords
    stop_words = set(stopwords.words("english"))

    # First filter out stopwords
    filtered_words = [word for word in word_tokens if word.lower() not in stop_words]

    # Remove ALL symbols/special characters AND numbers (only keep alphabetic characters)
    clean_words = []
    for word in filtered_words:
        # Only keep alphabetic characters (letters only, no numbers)
        clean_word = "".join(char for char in word if char.isalpha())
        if clean_word:  # Only add non-empty strings
            clean_words.append(clean_word.lower())

    truncated = clean_words[:min(len(clean_words), limit)]

    # Join the words back into text
    processed_text = " ".join(truncated)

    # Write to output file
    try:
        with open(output_file, "w", encoding="utf-8") as file:
            file.write(processed_text)
        print(f"Processed text saved to '{output_file}'")
        print(f"All numbers, symbols, and stopwords have been removed.")
    except Exception as e:
        print(f"Error writing to output file: {e}")


if __name__ == "__main__":
    # Set up command-line argument parsing
    parser = argparse.ArgumentParser(
        description="Process text by removing stopwords, numbers, and ALL symbols."
    )
    parser.add_argument("input", help="Input file path")
    parser.add_argument("output", help="Output file path")
    parser.add_argument("limit", help="Truncate test case to at most n words", type=int)

    args = parser.parse_args()

    # Process the text
    process_text(args.input, args.output, args.limit)
