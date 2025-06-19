if [ -z "$1" ]; then
    echo -e "\033[31mUSO INCORRETO DO COMANDO!\033[0m"
    echo -e "+---------------------------+-------------------------------------------------------+"
    echo -e "| \033[33mEntrada\033[0m                   | \033[33mSaida\033[0m                                                 |"
    echo -e "+---------------------------+-------------------------------------------------------+"
    echo -e "| \033[33m./compile.sh\033[0m              | \033[33mCompila, roda e exclui executavel em seguida\033[0m          |"
    echo -e "| \033[33m.sh + <nome_do_arquivo>\033[0m   | \033[33midem e utiliza arquivo passado para inicializar grafo\033[0m |"
    echo -e "+---------------------------+-------------------------------------------------------+"
    echo -e "| \033[32m./compile.sh grafo01.txt\033[0m  | \033[32mRoda programa utilizando o grafo01 como base\033[0m          |"
    echo -e "+---------------------------+-------------------------------------------------------+"

else
    echo "COMPILANDO..."
    cd src/
    g++ *.cpp -o execGrupoX
    echo "INICIANDO"
    ./execGrupoX "$1"
    rm execGrupoX
    cd ../
fi