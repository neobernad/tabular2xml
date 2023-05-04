# Converter

Run as:

`./converter -i input.tsv -o output.xml`

Default delimiter is ',', you may change this behaviour with the argument `col-divider`.

A config file with `-c` can be especified:

`./converter -c config.cfg -i input.tsv -o output.xml`

And `config.cfg` can set the following parameters:

```bash
col-names=aux0,aux1,aux2,aux3
col-values=+,*,+,*
has-header=true
#item-tag=Item # Commented line
#root-tag=RootItem
col-division=primaryProfession, knownForTitles #name.basics
#col-division=genres # title.basics
#col-division=directors, writers # title.crew
col-divider=","
```

## Compiling

### Release version

Go to `converter/Release/` and execute `make`

### Debug version

Go to `converter/Debug/` and execute `make`
