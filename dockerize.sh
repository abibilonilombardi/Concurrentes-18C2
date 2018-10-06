docker build -t gcc_debian . 
docker run -it -v $(pwd):/workspace gcc_debian /bin/bash
