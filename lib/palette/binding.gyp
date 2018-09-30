{
    "targets": [{
        "target_name": "palette",
        "include_dirs": [
            "src",
            "<!(node -e \"require('nan')\")"
        ],
        "sources": [
            "src/index.cc",
            "src/ColorCutQuantizer.cpp",
            "src/Palette.cpp",
            "src/Swatch.cpp",
            "src/sync.cpp",
            "src/Target.cpp"
        ],
        "conditions": [
            ['OS=="win"', {
                'defines': [
                    'NOMINMAX'
                ]
            }]
        ]
    }]
}