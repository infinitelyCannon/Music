{
    "targets": [{
        "target_name": "palette",
        "include_dirs": [
            "src",
            "<!(node -e \"require('nan')\")"
        ],
        "sources": [
            "src/index.cc"
        ]
    }]
}