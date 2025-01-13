# Define the default header search paths
HEADER_SEARCH_PATHS=(
    "/Users/treja/metal-cpp"
    "/Users/treja/metal-cpp-extensions"
    "/Users/treja/metal_playground/ModelIO_CPP"
)

# Convert the array into a space-separated string with quotes
HEADER_PATHS_STRING=$(printf " \"%s\"" "${HEADER_SEARCH_PATHS[@]}")

# Find all .xcodeproj files and process each one
find . -name "*.xcodeproj" | while read proj; do
    echo "Processing project: $proj"
    PROJECT_FILE="$proj/project.pbxproj"

    # Check if HEADER_SEARCH_PATHS key exists
    if grep -q "HEADER_SEARCH_PATHS" "$PROJECT_FILE"; then
        echo "Updating HEADER_SEARCH_PATHS in $PROJECT_FILE"
        # Update existing HEADER_SEARCH_PATHS
        sed -i "" "s|HEADER_SEARCH_PATHS = .*|HEADER_SEARCH_PATHS = ($HEADER_PATHS_STRING );|" "$PROJECT_FILE"
    else
        echo "Adding HEADER_SEARCH_PATHS to $PROJECT_FILE"
        # Add HEADER_SEARCH_PATHS if it doesn't exist
        sed -i "" "/buildSettings = {/a\\
        HEADER_SEARCH_PATHS = ($HEADER_PATHS_STRING );
        " "$PROJECT_FILE"
    fi
done

echo "Header search paths applied to all projects!"
