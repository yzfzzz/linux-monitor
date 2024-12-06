if [ -d "logs" ]; then
    echo "logs directory already exists."
    rm -r logs
else
    echo "Created logs directory."
fi

mkdir logs

if [ -d "./utils/py_cpp_pipe.fifo" ]; then
    echo "Build directory already exists."
    rm -r ./utils/py_cpp_pipe.fifo
fi

python3 ./utils/write.py & ./monitor -i address.conf