#!/bin/bash

path="$(dirname "$0" | sed s/' '/'\\ '/g)"
main="$path/app/build/distributions/app/bin/app"

version() {
    printf "Sea 0.0.0\n"
}

update() {
    printf "Not Implemented\n"
}

printu() {
    printf "  %-20s" "$1"
    printf "\t%-52s\n" "$2"
}

usage() {
    printf "Usage: sea [OPTIONS]... [DIR|FILE]... \n"
    printf "Compiles Sea code in FILEs or in DIRs and subdirs.\n\n"
    printf "OPTIONS:\n"

    printu "-d, --debug" "writes compiler debug information to a file."
    printu "-h, --help" "prints the sea command's usage information."
    printu "-u, --update" "uses git to update Sea to the latest version."
    printu "--version" "prints the sea command's version information."
    printf "\n"

    printu "-o, --out=OUT" "specifies the output directory to compile to;"
    printu "" "all files will be written into OUT, which is"
    printu "" "the current directory by default; The directory"
    printu "" "will be created if it does not exist."
}

options="-"
out_dir="."
files=()

add() {
    options="${options}$1"
}

get_arg_return=""

get_arg() {
    if [[ "${OPTARG}" == *"="* ]]
    then
        get_arg_return="${OPTARG#*=}"
        return 0
    fi

    printf "Must supply an argument to %s.\n" "$1"
    exit 1
}

get_single_arg() {
    get_arg "$1"

    char="${get_arg_return::1}"

    if [[ "$2" == "" || "$2" == *"$char"* ]]
    then
        get_arg_return="$char"
        return 0
    fi

    printf "Argument must be one of '%s'.\n" "$2"
    exit 3
}

while getopts ":-:hduo:" arg
do
    case "${arg}" in
        -)
            case "${OPTARG}" in
                "help")
                    usage
                    exit 0 ;;
                "debug")
                    add "d" ;;
                "update")
                    update
                    exit "$?" ;;
                "version")
                    version
                    exit 0 ;;
                "out"|"out="*)
                    get_arg "--out"
                    out_dir="$get_arg_return" ;;
                *)
                    printf "Invalid options: --%s.\n" "${OPTARG}"
                    exit 2 ;;
            esac ;;
        "h")
            usage
            exit 0 ;;
        "d")
            add "d" ;;
        "u")
            update
            exit "$?" ;;
        "o")
            get_arg "-o"
            out_dir="$get_arg_return" ;;
        :)
            printf "Must supply an argument to -%s.\n" "${OPTARG}"
            exit 1 ;;
        ?)
            printf "Invalid options: -%s.\n" "${OPTARG}"
            exit 2 ;;
    esac
done

for (( i=1; i<= "$#"; i++ ))
do
    [[ "${!i}" == -* ]] && continue

    if [[ ! -f "${!i}" && ! -d "${!i}" ]]
    then
        printf "'%s' is not an existing file or directory.\n" "${!i}"
        exit 4
    fi

    hy_files=()
    mapfile -d '' hy_files < <(find "${!i}" -type f -name "*.sea" -print0)
    files+=("${hy_files[@]}")
done

if [[ "${files[*]}" == "" ]]
then
    printf "Requires input files or directories to compile.\n"
    exit 6
fi

mapfile -d '' files < <(printf "%s\0" "${files[@]}" | sort -uz)
mkdir -p "$out_dir"
out_dir="$(realpath --relative-base=. "$out_dir")"

printf "\n"

if ! eval "$main" "$options" "$out_dir" "${files[*]@Q}"
then
    exit "$?"
fi

exit 0
