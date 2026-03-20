from definitions import factory, definition
import os
import sys
  

if (len(sys.argv) != 2):
    print("Usage: python populate_definitions.py <root_directory>")
    sys.exit()
    
root: str = sys.argv[1]
definitions_dir = os.path.join(root, "scripts", "data", "definitions")
destination = os.path.join(root, "src", "program", "definitions")

definitions = definition.generate(
      definitions_dir,
      destination     
)

factory.generate(
      definitions_dir,
      definitions,
      destination
)

print("Definitons OK.")