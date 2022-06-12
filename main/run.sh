loop=0
echo "Welcome to my dealer launcher..."
echo "Development done by Dimitar Milchev"
echo "Enjoy"

while [ $loop -le 1 ]
    do
    echo "1) Start the application"
    echo "2) Swap the backgrounds"
    echo "3) View source code"
    echo "4) Get callendar"
    echo "5) Exit"
    read op

    case $op in 
            1)
                ./main/main
            ;;
            2)
                external/background/scr/swap.sh
            ;;
            3)
                cat main/main.c
            ;;
            4)
                cal
            ;;
            5)
                exit
            ;;
        esac
    done
exit